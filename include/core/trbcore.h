#pragma once

/*
 * Execute a shell command.
 *
 * Parses and runs the command given in cmd_buffer,
 * using the provided environment variables envp.
 *
 * @param cmd_buffer Command string to execute.
 * @param envp Null-terminated array of environment variables.
 * @return 0 on success, -1 on failure.
 */
int trbshell_exec(char *cmd_buffer, char **envp);