#pragma once

/*
 * Retrieve the value of an environment variable by key.
 *
 * @param buffer Destination buffer to store the value.
 * @param size Size of the buffer.
 * @param key Environment variable name to look for.
 * @param envp Null-terminated array of environment strings ("KEY=VALUE").
 * @return 0 on success, -1 if not found or on error.
 */
int trb_getenv(char *buffer, int size, const char *key, char **envp);