#pragma once

// Standard file descriptors
#define EOF      -1
#define STDIN_FD  0
#define STDOUT_FD 1
#define STDERR_FD 2

// Null pointer definition
#define NULL (void*)0

// File open flags (matching Linux syscall flags)
enum flags {
    O_RDONLY = 0x000,   // Open for reading only
    O_WRONLY = 0x001,   // Open for writing only
    O_RDWR   = 0x002,   // Open for reading and writing
    O_CREAT  = 0x040,   // Create file if it does not exist
    O_EXCL   = 0x080,   // Error if O_CREAT and file exists
    O_TRUNC  = 0x200,   // Truncate file upon open
    O_APPEND = 0x400    // Append on each write
};

// Access mode flags for trb_access()
enum access_modes {
    F_OK = 0,           // Check for existence of file
    X_OK = 1,           // Check for execute permission
    W_OK = 2,           // Check for write permission
    R_OK = 4            // Check for read permission
};

// Write a null-terminated string to stdout
int trb_print(const char *buff);

// Write a null-terminated string to stderr
int trb_printerr(const char *buff);