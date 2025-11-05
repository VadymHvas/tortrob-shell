#include "lib/trbstdio.h"
#include "lib/trbenv.h"
#include "lib/trbstring.h"
#include "lib/trbmem.h"
#include "asm/trbsyscall.h"
#include "core/builtin/trbbuiltin_exec.h"
#include "core/history/trbhistory.h"
#include "core/trbcore.h"
#include "core/trbcmd.h"

#define PATH_SIZE 1024 // Maximum buffer of PATH size

void trb_execvp(char **args, char **envp)
{
    // Child proccess
    char path_env[PATH_SIZE];

    // Get PATH environment variable
    if (trb_getenv(path_env, PATH_SIZE, "PATH", envp) == -1) {
        trb_printerr("failed to get PATH");
        trb_exit(1);
    }

    // Split PATH into directories
    char **dirs = trb_strsplit(path_env, ':');

    for (int i = 0; dirs[i]; i++) {
        char fullpath[512];

        // Build full path: dir + "/" + command
        trb_strncpy(fullpath, dirs[i], 512);
        trb_strcat(fullpath, "/");
        trb_strcat(fullpath, args[0]);

        // Check if file exists and executable
        if (trb_access(fullpath, X_OK) == 0) {
            // Execute command
            trb_execve(fullpath, args, envp);

            trb_exit(1);
        }
    }

    // Command not found
    trb_printerr(args[0]);
    trb_printerr(": no such file or command");
    trb_printerr("\n");

    trb_exit(1);
}

/*
 * Main shell execution function.
 * Parses the command buffer.
 * Executes built-in commands directly.
 */
int trbshell_exec(char *cmd_buffer, char **envp)
{
    char **args = trb_parse_args(cmd_buffer);

    // Check for an empty command
    if (!args || !args[0] || args[0][0] == '\n')
        return -1;
    
    struct trb_command_t *cmd = trb_cmd_alloc(args);

    if (cmd == NULL)
        return -1;

    if (trb_parse_pipeline(cmd) == -1) {
        trb_printerr("failed to parse pipeline");
        return -1;
    }

    // Write command to history
    if (trb_write_to_history(cmd_buffer, envp) == -1)
        trb_printerr("failed to write command into history");

    // Try to execute builtin command
    if (trbbuiltin_exec(cmd->args, envp) == 0)
        return 0;

    // After pipeline parsing, if command has pipes field next will not be equal to NULL
    if (cmd->next != NULL) {
        trb_exec_pipeline(cmd, envp);
    } else {
        // fork() proccess to execute external command
        int proc = trb_fork();
        if (proc == -1) {
            return -1;
        } else if (proc == 0) {
            trb_execvp(cmd->args, envp);
        }

        // Wait for the proccess
        int status = 0;
        trb_waitpid(-1, &status, 0);
    }

    trb_free_cmd(cmd);

    return 0;
}