#include "lib/trbenv.h"
#include "lib/trbstring.h"
#include "lib/trbstdio.h"
#include "asm/trbsyscall.h"
#include "core/history/trbhistory.h"

/* Writes command into history */
int trb_write_to_history(char **args, char **envp)
{
    char home[HOME_SIZE];                  // Buffer to store HOME path
    char history_path[HISTORY_PATH];      // Buffer for full history file path
    int fd;

    // Retrieve the HOME environment variable
    if (trb_getenv(home, HOME_SIZE, "HOME", envp) == -1)
        return -1;

    // Construct the full history file path: $HOME/HISTORY_FILENAME
    trb_strcpy(history_path, home);
    trb_strcat(history_path, "/");
    trb_strcat(history_path, HISTORY_FILENAME);

    // Open history file with write, create, and append flags
    fd = trb_open(history_path, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1)
        return -1;

    // Write each argument to the history file, separated by spaces
    for (int i = 0; args[i] != NULL; i++) {
        trb_write(fd, args[i], trb_strlen(args[i]));

        if (args[i + 1] != NULL)
            trb_write(fd, " ", 1);
    }

    // Write a newline character at the end
    trb_write(fd, "\n", 1);

    // Close the file descriptor
    trb_close(fd);

    return 0;
}