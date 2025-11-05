#include "lib/trbstdio.h"
#include "lib/trbstring.h"
#include "core/trbusr.h"
#include "core/trbcore.h"
#include "asm/trbsyscall.h"
#include "lib/trbmem.h"

#define PROMPT_SIZE     512  // Maximum prompt length
#define CMD_BUFFER_SIZE 2048 // Maximum buffer size

/* 
 * Dynamic shell prompt:
 * user@hostname cwd(# or $)>
 */
static char trb_prompt[PROMPT_SIZE];
/*
 * Startup banner: display when shell launch.
 * Philosophy: minimalism, C + asm, zero libc, maximum tortrobs.
 */
static const char *trb_banner = "[_____________Welcome to Tortrob shell v1.0.0_____________]\n"
                                "[_________Written in pure C and Assembly, no libc_________]\n\n";

/*
 * Main loop in the shell
 * Print prompt;
 * Read user input;
 * Executes command;
 * Repeat
 */
int main(int argc, char **argv, char **envp)
{
    char cmd_buffer[CMD_BUFFER_SIZE];

    // Print banner, welcome to tortrob shell!
    trb_print(trb_banner);

    while (1) {
        // Get prompt; exit on fail
        if (trb_get_prompt(trb_prompt, PROMPT_SIZE, envp) == -1) {
            trb_printerr("failed to get prompt\n");
            trb_exit(1);
        }
        
        trb_print(trb_prompt);

        // Read user input into buffer
        trb_read(STDIN_FD, cmd_buffer, CMD_BUFFER_SIZE);

        int cmd_len = trb_strlen(cmd_buffer);

        // Remove trailing newline
        if (cmd_len > 0 && cmd_buffer[cmd_len - 1] == '\n')
            cmd_buffer[cmd_len - 1] = 0;

        // Execute command; 
        trbshell_exec(cmd_buffer, envp);

        // Clear buffer for next command;
        for (int i = 0; i < CMD_BUFFER_SIZE; i++)
            cmd_buffer[i] = 0;
    }

    return 0; // Unreachable;
}