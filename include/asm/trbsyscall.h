#pragma once
/**
 * @brief Minimal syscall interface definitions for a libc-free environment.
 *
 * This header defines basic memory mapping flags, argument structures,
 * and syscall-like wrappers used throughout the shell project. These
 * functions provide access to kernel functionality without relying on
 * the standard C library (libc).
 *
 * All `trb_*` functions are thin wrappers around system calls implemented
 * via syscall trampoline.
 */

/*
 * Memory protection flags for mmap() system call.
 * Used to specify access permissions on mapped memory regions.
 */
enum mmap_prot_flags {
    PROT_NONE  = 0x0,
    PROT_READ  = 0x1,
    PROT_WRITE = 0x2,
    PROT_EXEC  = 0x4
};

/*
 * Mapping flags for mmap() system call.
 * Used to control visibility and allocation behavior of mappings.
 */
enum mmap_map_flags {
    MAP_SHARED    = 0x01,
    MAP_PRIVATE   = 0x02,
    MAP_ANONYMOUS = 0x20
};

/*
 * mmap argument struct
 * Used as input to trb_mmap() instead of direct syscall parameters.
 */
struct mmap_args {
    unsigned long addr;
    unsigned long len;
    unsigned long prot;
    unsigned long flags;
    unsigned long fd;
    unsigned long offset;
};

/** Write data to a file descriptor. Returns number of bytes written or -1. */
extern int trb_write(int fd, const char *buff, int size);

/** Read data from a file descriptor. Returns number of bytes read or -1. */
extern int trb_read(int fd, char *buff, int size);

/** Execute a program, replacing the current process image. */
extern int trb_execve(const char *path, char *const argv[], char *const envp[]);

/** Wait for a child process to change state. Returns child PID or -1. */
extern int trb_waitpid(int pid, int *status, int options);

/** Check accessibility of a file (mode like F_OK, R_OK, W_OK, X_OK). */
extern int trb_access(const char *path, int mode);

/** Get current working directory. */
extern int trb_getcwd(char *buff, int size);

/** Open a file with given flags and mode. Returns a file descriptor or -1. */
extern int trb_open(const char *path, int flags, int mode);

/** Close an open file descriptor. */
extern int trb_close(int fd);

/** Exit current process with a given status code. */
extern int trb_exit(int status);

/** Change current working directory. */
extern int trb_chdir(const char *path);

/** Duplicate a file descriptor (oldfd → newfd). */
extern int trb_dup2(int oldfd, int newfd);

/** Create a new process (returns 0 in child, PID in parent, or -1). */
extern int trb_fork();

/** Get the real user ID of the calling process. */
extern int trb_getuid();

/** Create a unidirectional pipe (pipefd[0] for read, pipefd[1] for write). */
extern int trb_pipe(int pipefd[2]);

/** Memory mapping function, similar to mmap(). */
extern void *trb_mmap(struct mmap_args *mmap_args);