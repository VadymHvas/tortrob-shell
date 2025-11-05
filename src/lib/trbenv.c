#include "lib/trbenv.h"
#include "lib/trbstdio.h"
#include "lib/trbstring.h"

/*
 * Get the value of an environment variable by key.
 * Arguments:
 *  buff - destination buffer to store the value
 *  size   - size of the buffer
 *  key    - environment variable name to search for
 *  envp   - array of environment strings (key=value)
 *
 * Returns:
 *  0  - success, value stored in buffer
 * -1  - failure (invalid args or key not found)
 */
int trb_getenv(char *buff, int size, const char *key, char **envp) {
    if (!buff || size <= 0 || !key || !envp) 
        return -1;

    // Iterate over environment variables
    for (int i = 0; envp[i] != 0; i++) {
        const char *entry = envp[i];
        int j = 0;

        // Compare the key with the beginning of the env entry
        while (key[j] && entry[j] && key[j] == entry[j])
            j++;

        // Check for exact match and '=' following the key
        if (key[j] == 0 && entry[j] == '=') {
            const char *val = entry + j + 1;

            // Copy value into buffer, ensure null termination
            int k = 0;
            while (val[k] && k < size - 1) {
                buff[k] = val[k];
                k++;
            }

            buff[k] = 0;  // Null-terminate
            return 0;
        }
    }

    return -1;  // Key not found
}