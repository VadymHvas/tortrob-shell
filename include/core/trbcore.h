#pragma once

/**
 * @brief Core header file for the Tortrob shell.
 *
 * Provides a lightweight wrapper for executing a program by searching
 * it's name in the PATH environment variable and core function trbshell_exec which
 * parses arguments, pipelines, writes command to the command history, executes
 * built-in commands, waits for programs, and frees resources.
 *
 */

/**
 * @brief Execute a program by searching it in the PATH.
 *
 * Attempts to execute the file named in `args[0]`, using the PATH
 * environment variable to resolve the full path if necessary.
 * On success, this call never returns.
 *
 * @param args  NULL-terminated argument vector (argv-like).
 *              args[0] should contain the program name or path.
 * @param envp  NULL-terminated environment array (passed to execve()).
 *
 * @note This function does not return on success. If execution fails,
 *       it may return to the caller or exit the process depending on
 *       implementation.
 */
void trb_execvp(char **args, char **envp);

/**
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