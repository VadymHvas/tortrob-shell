#include "core/builtin/trbbuiltin_exec.h"
#include "core/builtin/trbbuiltin_cmd.h"
#include "lib/trbstring.h"
#include "lib/trbstdio.h"

struct builtin {
    const char *key;
    int (*func) (char **args, char **envp);
};

// List of builtin commands.
// Commands "exit", "quit", and "q" map to the same function.
struct builtin builtin_list[] = {
    { "history", trb_history },
    { "exit",    trb_sh_exit },
    { "quit",    trb_sh_exit },
    { "q",       trb_sh_exit },
    { "cd",      trb_cd },
    { NULL,      NULL }  // Terminator for the list
};

/*
 * Execute a builtin command if it matches args[0].
 * Returns:
 *  0  - command was a builtin and executed successfully
 * -1  - not a builtin, or execution failed
 */
int trbbuiltin_exec(char **args, char **envp) {
    if (!args || !args[0])  // Defensive check
        return -1;

    // Search the builtin list for a matching command
    for (int i = 0; builtin_list[i].key; i++) {
        if (trb_strcmp(args[0], builtin_list[i].key) == 0) {
            // Execute the corresponding function
            if (builtin_list[i].func(args, envp) == -1)
                return -1;

            return 0;  // Command handled
        }
    }

    return -1;  // Not a builtin
}