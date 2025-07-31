#include "lib/trbenv.h"
#include "lib/trbstdio.h"
#include "lib/trbstring.h"

#define MAX_TOKENS     128
#define MAX_TOKENS_LEN 256

int trb_strlen(const char *str)
{
    int i = 0;

    while (str[i]) {
        i++;
    }

    return i;
}

int trb_strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }

    return (unsigned char)*str1 - (unsigned char)*str2;
}

int trb_strncmp(const char *str1, const char *str2, int n)
{
    int i = 0;

    while (i < n) {
        if (*str1 != *str2)
            return (unsigned char)*str1 - (unsigned char)*str2;

        if (*str1 == '\0') 
            return 0;
            
        str1++;
        str2++;
        i++;
    }

    return 0;
}

char *trb_strcpy(char *dest, const char *src)
{
    int i = 0;
    
    while (src[i] != 0) {
        dest[i] = src[i];
        i++;
    }

    dest[i] = 0;
    return dest;
}

char *trb_strncpy(char *dest, const char *src, int n)
{
    int i = 0;

    while (i < n && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    while (i < n) {
        dest[i] = '\0';
        i++;
    }

    return dest;
}

char *trb_strcat(char *dest, const char *src)
{
    int i = 0, j = 0;

    while (dest[i] != '\0') {
        i++;
    }

    while (src[j] != '\0') {
        dest[i + j] = src[j];
        j++;
    }

    dest[i + j] = '\0';

    return dest;
}

int trb_pow(int a, int b)
{
    for (int i = 0; i < b; i++) {
        a *= b;
    }

    return a;
}

unsigned int trb_atoi(const char *str)
{
    int num = 0;
    int i = 0;

    while (*str) {
        if (*str < '0' || *str > '9') {
            return -1;
        }

        num += (*str - '0') * trb_pow(10, i);

        i++;
    }

    return num;
}

char **trb_strsplit(char *str, char delim)
{
    static char *tokens[MAX_TOKENS];
    int token_i = 0;
    int in_token = 0;

    for (int i = 0; str[i] != 0; i++) {
        if (str[i] == delim) {
            str[i] = 0;
            in_token = 0;
        } else if (!in_token) {
            if (token_i >= MAX_TOKENS) break;
            tokens[token_i++] = &str[i];
            in_token = 1;
        }
    }

    tokens[token_i] = NULL;
    return tokens;
}