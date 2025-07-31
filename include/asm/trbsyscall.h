#pragma once

extern int trb_write(int fd, const char *buff, int size);
extern int trb_read(int fd, char *buff, int size);
extern int trb_execve(const char *path, char *const argv[], char *const envp[]);
extern int trb_waitpid(int pid, int *status, int options);
extern int trb_access(const char *path, int mode);
extern int trb_getcwd(char *buff, int size);
extern int trb_open(const char *path, int flags, int mode);
extern int trb_close(int fd);
extern int trb_exit(int status);
extern int trb_chdir(const char *path);
extern int trb_dup2(int oldfd, int newfd);
extern int trb_fork();
extern int trb_getuid();