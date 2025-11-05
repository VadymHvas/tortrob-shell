#include "lib/trbstring.h"
#include "lib/trbstdio.h"
#include "lib/trbenv.h"
#include "asm/trbsyscall.h"
#include "core/builtin/trbbuiltin_cmd.h"
#include "core/history/trbhistory.h"

/*
 * Exit the shell.
 * If args[1] is provided, it is used as the exit status.
 * Otherwise, exit with status 0.
 */
int trb_sh_exit(char **args, char **envp)
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
int trb_cd(char **args, char **envp)
{
    if (args[1] == NULL) {
        // trb_print("вкажи шлях дурік\n"); А ти шо коваль

        char home[HOME_SIZE];

        if (trb_getenv(home, HOME_SIZE, "HOME", envp) == -1)
            return -1;

        trb_chdir(home);
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

/*
 * Opens the user's history file (located in $HOME) and prints its contents
 * to standard output. If the history file does not exist, it will be created
 * but remain empty. The function reads in fixed-size chunks.
*/
int trb_history(char **args, char **envp)
{
    char home[HOME_SIZE];
    char buffer[HISTORY_BUFFER_SIZE];
    char history_path[HISTORY_PATH_SIZE];
    int bytes_read;
    int fd;

    if (trb_getenv(home, HOME_SIZE, "HOME", envp) == -1)
        return -1;

    trb_strcpy(history_path, home);
    trb_strcat(history_path, "/");
    trb_strcat(history_path, HISTORY_FILENAME);

    fd = trb_open(history_path, O_RDONLY | O_CREAT, 0666);

    if (fd == -1)
        return -1;

    while ((bytes_read = trb_read(fd, buffer, HISTORY_BUFFER_SIZE)) > 0)
        trb_print(buffer);

    return 0;
}