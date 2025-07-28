#pragma once

int trb_write(int fd, const char *buff, int size);
int trb_read(int fd, char *buff, int size);
int trb_execve(const char *path, char *const argv[], char *const envp[]);
int trb_waitpid(int pid, int *status, int options);
int trb_access(const char *path, int mode);
int trb_getcwd(char *buff, int size);
int trb_open(const char *path, int flags, int mode);
int trb_close(int fd);
int trb_exit(int status);
int trb_chdir(const char *path);
int trb_fork();
int trb_getuid();