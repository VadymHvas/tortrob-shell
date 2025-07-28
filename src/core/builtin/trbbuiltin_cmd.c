#include "core/builtin/trbbuiltin_cmd.h"
#include "lib/trbstring.h"
#include "lib/trbstdio.h"
#include "asm/trbsyscall.h"

/*
 * Exit the shell.
 * If args[1] is provided, it is used as the exit status.
 * Otherwise, exit with status 0.
 */
int trb_sh_exit(char **args)
{
    if (args[1] != NULL) {
        int status = trb_atoi(args[1]);

        trb_print("exit\n");
        trb_exit(status);
    }

    trb_print("exit\n");
    trb_exit(0);
}

/*
 * Change current working directory.
 * Usage: cd <path>
 * If no path is given, print a warning.
 * If path does not exist, print an error.
 */
int trb_cd(char **args)
{
    if (args[1] == NULL) {
        trb_print("вкажи шлях дурік\n"); // А ти шо коваль
    } else {
        int chdir_ok = trb_chdir(args[1]);
        if (chdir_ok == -1) {
            trb_print("cd: directory '");
            trb_print(args[1]);
            trb_print("' does not exist\n");
        }
    }

    return 0;
}
