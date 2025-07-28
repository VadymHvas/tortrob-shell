#pragma once

int trb_strlen(const char *str);
int trb_strcmp(const char *str1, const char *str2);
int trb_strncmp(const char *str1, const char *str2, int n);
unsigned int trb_atoi(const char *str);
char *trb_strcpy(char *dest, const char *src);
char *trb_strncpy(char *dest, const char *src, int n);
char *trb_strcat(char *dest, const char *src);
char **trb_strsplit(char *str, char delim);