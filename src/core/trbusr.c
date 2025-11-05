#include "asm/trbsyscall.h"
#include "core/trbusr.h"
#include "lib/trbstdio.h"
#include "lib/trbenv.h"
#include "lib/trbstring.h"

#define HOSTNAME_SIZE 64
#define USERNAME_SIZE 64
#define HOME_SIZE     128
#define CWD_SIZE      512

/*
 * Reads the system hostname from /etc/hostname into the given buffer.
 */
static int trb_get_hostname(char *buffer, int size)
{
    int fd = trb_open("/etc/hostname", O_RDONLY, 0);
    if (fd == -1)
        return -1;

    if (trb_read(fd, buffer, size) == -1)
        return -1;

    trb_close(fd);
    return 0;
}

/*
 * Constructs a formatted shell prompt string.
 *
 * Format:
 *   username@hostname cwd [#|$]> 
 *
 * If the current working directory starts with $HOME,
 * it is replaced with '~'. Root users (uid 0) get '#', others get '$'.
 *
 */
int trb_get_prompt(char *buffer, int size, char **envp)
{
    char hostname[HOSTNAME_SIZE];
    char username[USERNAME_SIZE];
    char cwd[CWD_SIZE];
    char home[HOME_SIZE];
    int is_root = trb_getuid();

    if (trb_get_hostname(hostname, HOSTNAME_SIZE) == -1)
        return -1;

    if (trb_getenv(username, USERNAME_SIZE, "USER", envp) == -1)
        return -1;

    if (trb_getcwd(cwd, CWD_SIZE) == -1)
        return -1;

    if (trb_getenv(home, HOME_SIZE, "HOME", envp) == -1)
        return -1;

    // Replace $HOME in cwd with '~' if cwd starts with home
    if (trb_strncmp(cwd, home, trb_strlen(home)) == 0) {
        char new_cwd[CWD_SIZE];

        new_cwd[0] = '~';
        trb_strcpy(&new_cwd[1], cwd + trb_strlen(home));
        trb_strcpy(cwd, new_cwd);
    }

    if (is_root == -1)
        return -1;

    int pos = 0;

    // Append username
    for (int i = 0; username[i] != 0 && pos < size - 1; ++i)
        buffer[pos++] = username[i];

    // Append '@'
    if (pos < size - 1)
        buffer[pos++] = '@';

    // Append hostname (up to newline)
    for (int i = 0; hostname[i] != '\n' && pos < size - 1; ++i)
        buffer[pos++] = hostname[i];

    // Append space
    buffer[pos++] = ' ';

    // Append cwd
    for (int i = 0; cwd[i] != 0 && pos < size - 1; ++i)
        buffer[pos++] = cwd[i];

    // Append '#' for root, '$' for others
    if (is_root == 0)
        buffer[pos++] = '#';
    else
        buffer[pos++] = '$';

    // Append '> '
    if (pos < size - 1)
        buffer[pos++] = '>';
    if (pos < size - 1)
        buffer[pos++] = ' ';

    // Null-terminate the string
    buffer[pos] = 0;

    return 0;
}
