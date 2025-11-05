#pragma once
/**
 * @brief Minimal string and conversion utilities for libc-free shell.
 *
 * This header defines a lightweight set of string manipulation and
 * conversion functions reimplemented from the C standard library.
 * Used throughout the project to avoid dependency on libc.
 *
 * All functions operate on null-terminated strings and avoid dynamic
 * memory allocations unless explicitly noted.
 */

/**
 * @brief Get the length of a null-terminated string.
 *
 * @param str  Input string.
 * @return Number of characters (excluding '\0').
 */
int trb_strlen(const char *str);

/**
 * @brief Compare two strings lexicographically.
 *
 * @param str1  First string.
 * @param str2  Second string.
 * @return 0 if equal, <0 if str1 < str2, >0 if str1 > str2.
 */
int trb_strcmp(const char *str1, const char *str2);

/**
 * @brief Compare at most n characters of two strings.
 *
 * @param str1  First string.
 * @param str2  Second string.
 * @param n     Maximum number of characters to compare.
 * @return 0 if equal up to n, <0 if str1 < str2, >0 if str1 > str2.
 */
int trb_strncmp(const char *str1, const char *str2, int n);

/**
 * @brief Convert a numeric string to an integer.
 *
 * @param str  Null-terminated numeric string.
 * @return Parsed unsigned integer value.
 */
unsigned int trb_atoi(const char *str);

/**
 * @brief Copy a null-terminated string.
 *
 * @param dest  Destination buffer.
 * @param src   Source string.
 * @return Pointer to dest.
 */
char *trb_strcpy(char *dest, const char *src);

/**
 * @brief Copy up to n characters from src into dest.
 *
 * @param dest  Destination buffer.
 * @param src   Source string.
 * @param n     Maximum number of characters to copy.
 * @return Pointer to dest.
 */
char *trb_strncpy(char *dest, const char *src, int n);

/**
 * @brief Concatenate two strings.
 *
 * @param dest  Destination buffer (modified in place).
 * @param src   Source string.
 * @return Pointer to dest.
 */
char *trb_strcat(char *dest, const char *src);

/**
 * @brief Split a string into tokens by a single delimiter.
 *
 * Replaces each occurrence of `delim` in `str` with '\0' and builds
 * a NULL-terminated array of pointers to individual tokens.
 *
 * Memory for the array of pointers is dynamically allocated.
 * Caller is responsible for freeing the returned array.
 *
 * @param str    Input string to split (modified in-place).
 * @param delim  Delimiter character.
 * @return NULL-terminated array of string pointers, or NULL on failure.
 */
char **trb_strsplit(char *str, char delim);