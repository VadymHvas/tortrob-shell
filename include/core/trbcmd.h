#pragma once
/**
 * @brief Command parsing and pipeline execution utilities for the Tortrob shell.
 *
 * This header defines core data structures and function prototypes
 * responsible for parsing command lines, building execution pipelines,
 * and invoking commands in a libc-free shell environment.
 *
 * Each command is represented as a linked list of `trb_command_t` nodes,
 * where each node holds its own argv-style argument list and a pointer
 * to the next command in the pipeline (if any).
 */

/*
 * 
 */
struct trb_command_t {
    char **args;                // NULL-terminated array of arguments (argv-like).
    struct trb_command_t *next; // Pointer to the next command in pipeline chain.
    /*
     * TODO: make a field that describes the array of redirects
     * and the redirect structure.
     */
};

/**
 * @brief Parse a command line buffer into an argument vector.
 *
 * Splits the given command string (cmd_buffer) into a NULL-terminated
 * array of tokens (arguments). Whitespace is used as a delimiter.
 *
 * @param cmd_buffer  Input command string (modified in-place).
 * @return Newly allocated NULL-terminated array of strings, or NULL on failure.
 */
char **trb_parse_args(char *cmd_buffer);

/**
 * @brief Parse a full command line containing a pipeline.
 *
 * Builds a linked list of `trb_command_t` structures, one per command
 * in the pipeline, connecting them via the `next` pointer.
 * Each element "|" in `args` replaces with NULL, which
 * allows to build an easy and efficient pipeline
 *
 * Example: "ls -l | grep txt | wc -l"
 * args: {"ls", "-l", NULL, "grep", "txt", NULL, "wc", "-l", NULL}
 *
 * @param cmd  Pointer to the first command node (allocated by caller).
 * @return 0 on success, nonzero on syntax/parse error.
 */
int trb_parse_pipeline(struct trb_command_t *cmd);

/**
 * @brief Execute a parsed command pipeline.
 *
 * For each command node in the chain, this function sets up pipes
 * as needed, forks child processes, and executes commands using execve().
 * The function blocks until all commands in the pipeline have finished.
 *
 * @param cmd   Pointer to the first command in the pipeline chain.
 * @param envp  Environment variables.
 */
void trb_exec_pipeline(struct trb_command_t *cmd, char **envp);

/**
 * @brief Free memory associated with a command pipeline.
 *
 * Releases all dynamically allocated argument arrays and command
 * structures linked via the `next` pointer.
 * Since the pointer to the args is stored only in the first command
 * int the pipeline, this function must be called for the first command.
 *
 * @param cmd  Pointer to the first command in the pipeline chain.
 */
void trb_free_cmd(struct trb_command_t *cmd);

/**
 * @brief Allocate and initialize a new command structure.
 *
 * Creates a new `trb_command_t` node with the provided argument vector.
 * The command’s `next` pointer is initialized to NULL.
 *
 * @param args  NULL-terminated argument vector (copied or owned by caller).
 * @return Pointer to a new command node, or NULL on allocation failure.
 */
struct trb_command_t *trb_cmd_alloc(char **args);