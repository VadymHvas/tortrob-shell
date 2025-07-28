#pragma once

/*
 * Builtin command: exit the shell.
 * 
 * @param args Arguments array; optional exit status in args[1].
 * @return Does not return, exits the process.
 */
int trb_sh_exit(char **args);

/*
 * Builtin command: change current working directory.
 * 
 * @param args Arguments array; expects directory path in args[1].
 * @return 0 on success, or non-zero on failure.
 */
int trb_cd(char **args);