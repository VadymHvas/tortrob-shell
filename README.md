# Tortrob Shell

Minimal shell written in C and Assembly without libc for Linux i386.

## Features
- Dynamic prompt.
- Builtin commands: cd, exit, etc.
- External command execution via execve.
- Custom syscall wrappers.

## Requirements
- Linux i386 environment
- NASM assembler
- GCC compiler
- ld.gold linker (or use the regular ld)

## Limitations
- No support for pipes, background processes yet.
- No command history implemented.

## Build
```$ make```

## Usage
```$ ./tortrob_shell```

## Clean
```$ make clean```

---

Happy hacking!