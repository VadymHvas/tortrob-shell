#include "lib/trbenv.h"
#include "lib/trbstdio.h"
#include "lib/trbstring.h"
#include "asm/trbsyscall.h"

// Writes into stdout
int trb_print(const char *buff)
{
    int buff_len = trb_strlen(buff);

    int write_ok = trb_write(STDOUT_FD, buff, buff_len);
    if (write_ok == -1)
        return -1;

    return buff_len;
}

// Writes into stderr
int trb_printerr(const char *buff)
{
    int buff_len = trb_strlen(buff);

    int write_ok = trb_write(STDERR_FD, buff, buff_len);
    if (write_ok == -1)
        return -1;

    return buff_len;
}