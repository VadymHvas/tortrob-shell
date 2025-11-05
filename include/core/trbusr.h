#pragma once

/**
 * Generate the shell prompt string dynamically.
 *
 * Constructs the prompt (e.g. user@host current_dir $/#)
 * and stores it in the provided buffer.
 *
 * @param buffer Destination buffer to store the prompt string.
 * @param size Size of the buffer.
 * @param envp Null-terminated array of environment variables.
 * @return 0 on success, -1 on failure.
 */
int trb_get_prompt(char *buffer, int size, char **envp);