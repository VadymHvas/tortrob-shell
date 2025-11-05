# Tortrob Shell v1.0.0

Minimal shell written in C and Assembly without libc for Linux i386.

## Features
- Dynamic prompt.
- Builtin commands: cd, exit, history.
- External command execution via execve.
- Custom syscall wrappers.
- Minimal command history.
- Pipes support.
- Memory allocator adapted for no libc.
- Parsing commands into arguments, taking into account quotes/tabs/newlines.

## Requirements
- Linux i386 environment
- NASM assembler
- GCC compiler
- ld.gold linker (or use the regular ld)

## Limitations
- No support for redirects and background processes yet.
- No integration of built-in commands with pipes.
- No Ctrl+C signal handling.
- No basics built-in commands like ```set```, ```export```, ```env```.
- etc.

## Build
Clone the repository.
```bash
git clone https://github.com/VadymHvas/tortrob-shell.git
```
```bash
cd tortrob_shell
```
Compile it.
```bash
make
```

## Usage
```bash
./tortrob_shell
```

## Clean
```bash
make clean
```

---

Happy hacking!