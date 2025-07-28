#include "lib/trbstdio.h"
#include "lib/trbenv.h"
#include "lib/trbstring.h"
#include "core/trbcore.h"
#include "core/builtin/trbbuiltin_exec.h"
#include "asm/trbsyscall.h"

#define PATH_SIZE 1024 // Maximum buffer of PATH size

/*
 * Parse command buffer into arguments.
 * Splits the string by spaces/tabs/newlines in-place.
 * Returns argv array (NULL-terminated).
 */
static char **trb_parse_args(char *cmd_buffer, char **argv, int max_args) {
    int argc = 0;
    int in_arg = 0;

    for (int i = 0; cmd_buffer[i] != '\0'; i++) {
        char c = cmd_buffer[i];

        if (c == ' ' || c == '\t' || c == '\n') {
            cmd_buffer[i] = '\0';
            in_arg = 0;
        } else if (!in_arg) {
            if (argc >= max_args)
                break;

            argv[argc++] = &cmd_buffer[i];
            in_arg = 1;
        }
    }

    argv[argc] = NULL;
    return argv;
}

/*
 * Main shell execution function.
 * Parses the command buffer.
 * Executes built-in commands directly.
 * Otherwise forks and executes external command from PATH.
 */
int trbshell_exec(char *cmd_buffer, char **envp)
{
    char *argv[128];
    char **args = trb_parse_args(cmd_buffer, argv, 128);

    // Check for an empty command
    if (!args || !args[0] || args[0][0] == '\n')
        return -1;

    // Try to execute builtin command
    if (trbbuiltin_exec(args) == 0)
        return 0;

    // fork() proccess to execute external command
    int proc = trb_fork();
    if (proc == -1) {
        return -1; // fork() failed
    } else if (proc == 0) {
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
            trb_strcpy(fullpath, dirs[i]);
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
    }

    // Wait for the proccess
    int status = 0;
    trb_waitpid(-1, &status, 0);

    return 0;
}