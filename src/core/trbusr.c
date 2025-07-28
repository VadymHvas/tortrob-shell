#include "asm/trbsyscall.h"
#include "core/trbusr.h"
#include "lib/trbstdio.h"
#include "lib/trbenv.h"

#define HOSTNAME_SIZE 64
#define USERNAME_SIZE 64
#define CWD_SIZE      512

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

int trb_get_prompt(char *buffer, int size, char **envp)
{
    char hostname[HOSTNAME_SIZE];
    char username[USERNAME_SIZE];
    char cwd[CWD_SIZE];
    int is_root = trb_getuid();

    if (trb_get_hostname(hostname, HOSTNAME_SIZE) == -1)
        return -1;

    if (trb_getenv(username, USERNAME_SIZE, "USER", envp) == -1)
        return -1;

    if (trb_getcwd(cwd, CWD_SIZE) == -1)
        return -1;

    if (is_root == -1)
        return -1;

    int pos = 0;

    for (int i = 0; username[i] != 0 && pos < size - 1; ++i)
        buffer[pos++] = username[i];

    if (pos < size - 1)
        buffer[pos++] = '@';

    for (int i = 0; hostname[i] != '\n' && pos < size - 1; ++i)
        buffer[pos++] = hostname[i];
    
    buffer[pos++] = ' ';

    for (int i = 0; cwd[i] != 0 && pos < size - 1; ++i)
        buffer[pos++] = cwd[i];

    if (is_root == 0)
        buffer[pos++] = '#';
    else
        buffer[pos++] = '$';

    if (pos < size - 1)
        buffer[pos++] = '>';

    if (pos < size - 1)
        buffer[pos++] = ' ';

    buffer[pos] = 0;
    return 0;
}