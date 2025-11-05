#pragma once

#define HISTORY_FILENAME      ".tortrob_history"
#define HISTORY_BUFFER_SIZE   1024
#define HISTORY_PATH_SIZE     256
#define HISTORY_PATH          256
#define HOME_SIZE             128

/**
 * @brief Writes the executed command to the shell history file.
 *
 * Constructs the path to the history file using the HOME environment variable
 * and appends the given command arguments to it.
 *
 * @param args Array of command arguments (null-terminated)
 * @param envp Environment pointer (null-terminated)
 * @return int 0 on success, -1 on failure
 */
int trb_write_to_history(char *buff, char **envp);