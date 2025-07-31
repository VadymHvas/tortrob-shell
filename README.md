# Tortrob Shell

Minimal shell written in C and Assembly without libc for Linux i386.

## Features
- Dynamic prompt.
- Builtin commands: cd, exit, history.
- External command execution via execve.
- Custom syscall wrappers.
- Minimal command history

## Requirements
- Linux i386 environment
- NASM assembler
- GCC compiler
- ld.gold linker (or use the regular ld)

## Limitations
- No support for pipes, background processes yet.

## Build
```git clone https://github.com/VadymHvas/tortrob-shell.git```
```cd tortrob_shell```
```make```

## Usage
```./tortrob_shell```

## Clean
```make clean```

---

Happy hacking!