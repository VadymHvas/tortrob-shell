#pragma once

/*
 * Execute a builtin command.
 *
 * @param args Null-terminated array of argument strings,
 *             where args[0] is the command name.
 * @return 0 if the command was handled successfully,
 *         -1 if the command was not found or failed.
 */
int trbbuiltin_exec(char **args, char **envp);