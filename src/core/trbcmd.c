#include "core/trbcmd.h"
#include "core/trbcore.h"
#include "lib/trbmem.h"
#include "lib/trbstring.h"
#include "lib/trbstdio.h"
#include "asm/trbsyscall.h"

#define MAX_HEAP_PTRS 128

/*
 * WORKAROUND: 
 * While developing trb_free_cmd() function, a bug was discovered
 * in coalescing free adjacent blocks, where under certain conditions
 * coalescing didn't work correctly, leading to the erroneous merging of the
 * entire heap size twice. 
 *
 * Therefore, to prevent this bug from occuring, an array of pointers
 * to "problematic" blocks that were freed under conditions where the bug occured was allocated. 
 
 * TODO:
 * - Fix coalescing routine to properly check neighboring block states.
 * - Remove this workaround once allocator logic is stable.
 */
static void *heap_ptrs[MAX_HEAP_PTRS];
static int heap_ptrs_count = 0;

/*
 * Parse command buffer into arguments.
 * Splits the string by spaces/tabs/newlines in-place.
 * Returns args array (NULL-terminated).
 */
char **trb_parse_args(char *cmd_buffer)
{
    if (!cmd_buffer)
        return NULL;

    int count = 0;
    char *ptr = cmd_buffer;

    // Counting the number of arguments
    while (*ptr) {
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
            ptr++;

        if (*ptr == '\0')
            break;

        count++;

        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n')
            ptr++;
    }

    char **args = (char **)trb_malloc(sizeof(char *) * (count + 1));
    int i = 0;
    int in_single_quote = 0, in_double_quote = 0;

    ptr = cmd_buffer;

    while (*ptr) {
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n')
            ptr++;

        if (*ptr == '\0')
            break;

        const char *start = ptr;
        char *out = ptr;

        while (*ptr) {
            if (in_single_quote == 0 && in_double_quote == 0) {
                if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    break;
                } else if (*ptr == '"') {
                    in_double_quote = 1;
                    ptr++;
                    continue;
                } else if (*ptr == '\'') {
                    in_single_quote = 1;
                    ptr++;
                    continue;
                }
            } else if (in_double_quote && *ptr == '"') {
                in_double_quote = 0;
                ptr++;
                continue;
            } else if (*ptr == '\'') {
                in_single_quote = 0;
                ptr++;
                continue;
            }

            *out++ = *ptr++;
        }

        int arg_len = out - start;
        char *arg = (char *)trb_malloc(arg_len + 1);

        if (!arg)
            return NULL;

        trb_strncpy(arg, start, arg_len + 1);

        arg[arg_len] = '\0';
        args[i++] = arg;
    }

    args[i] = NULL;
    return args;
}

void trb_free_cmd(struct trb_command_t *cmd)
{
    struct trb_command_t *curr = cmd;
    char **saved_args = cmd->args;

    while (curr != NULL) {
        struct trb_command_t *saved_next = curr->next;

        for (int i = 0; curr->args[i] != NULL; ++i)
            trb_free(curr->args[i]);
        
        trb_free(curr);

        curr = saved_next;
    }

    trb_free(saved_args);

    // Free all non-free "|" from the heap
    for (int i = 0; i < heap_ptrs_count; i++)
        trb_free(heap_ptrs[i]);

    // Clean array if too many pointers
    if (heap_ptrs_count == MAX_HEAP_PTRS)
        for (int i = 0; i < MAX_HEAP_PTRS; i++)
            heap_ptrs[i] = 0;
}

int trb_parse_pipeline(struct trb_command_t *cmd)
{
    for (int i = 0; cmd->args[i] != NULL; i++) {
        if (!trb_strcmp(cmd->args[i], "|")) {
           /*
            * WORKAROUND: Prevent incorrect coalescing for problematic block
            * ---------------------------------------------------------------
            * BUG: When freeing a block between two allocated blocks, the
            * coalescing routine incorrectly merged the block into the entire
            * heap. This condition was observed here, at this line:
            *    coalesce(current_block);
            *
            * WORKAROUND: Temporarily store the block pointer in heap_ptrs[]
            * to avoid merging the entire heap. This is a short-term fix.
            *
            * TODO (correct solution):
            * - Modify coalesce() to properly check the free status of neighbors
            *   before merging.
            * - Use trb_free(cmd->args[i]) instead of free pointer in the trb_cmd_free().
            * - Remove this heap_ptrs[] workaround once coalesce() is fixed.
            */
            heap_ptrs[heap_ptrs_count] = cmd->args[i]; // Save pointer, for the free in the trb_cmd_free
            heap_ptrs_count++;

            cmd->args[i] = NULL;

            if (cmd->args[i + 1] == NULL)
                return -1;

            cmd->next = trb_cmd_alloc(&cmd->args[i + 1]); // &cmd->args[i + 1] is a pointer to the next command args in pipeline.

            if (cmd->next == NULL)
                return -1;

            return trb_parse_pipeline(cmd->next); // Repeat this parsing algorithm for the next command in pipeline.
        }
    }

    return 0;
}

/*
 * Core command pipeline execution function.
 */
void trb_exec_pipeline(struct trb_command_t *cmd, char **envp)
{
    struct trb_command_t *curr = cmd;
    int prev_fd = -1;   // Holds the read end of the previous pipe
    int status = 0;
    int proc;

    while (curr != NULL) {
        int fd[2];  // Pipe for the current command, if needed

        if (curr->next != NULL)
            trb_pipe(fd);  // Create a pipe if this is not the last command

        proc = trb_fork();
        if (proc == 0) {
            // Child process

            if (prev_fd != -1) {
                // Redirect input from previous pipe
                trb_dup2(prev_fd, STDIN_FD);
                trb_close(prev_fd);
            }

            if (curr->next != NULL) {
                // Redirect output to current pipe
                trb_dup2(fd[1], STDOUT_FD);
                trb_close(fd[0]);
                trb_close(fd[1]);
            }

            trb_execvp(curr->args, envp);
        }

        // Parent process
        if (prev_fd != -1)
            trb_close(prev_fd);  // Close the read end of the previous pipe

        if (curr->next != NULL) {
            trb_close(fd[1]);  // Close write end in parent
            prev_fd = fd[0];   // Cave read end for the next command
        } else prev_fd = -1;

        curr = curr->next;
    }

    // Cleanup: close last read end if needed
    if (prev_fd != -1)
        trb_close(prev_fd);
    
    // Wait for all child processes
    while (trb_waitpid(-1, &status, 0) > 0)
        ;
}

struct trb_command_t *trb_cmd_alloc(char **args)
{
    struct trb_command_t *cmd = (struct trb_command_t *)trb_malloc(sizeof(struct trb_command_t));

    if (cmd == NULL)
        return NULL;

    cmd->args = args;
    cmd->next = NULL;

    return cmd;
}