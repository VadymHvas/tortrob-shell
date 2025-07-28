AS = nasm
CC = gcc
LD = ld.gold

ASFLAGS = -f elf32
CFLAGS = -m32 -nostdlib -fno-stack-protector -fno-builtin -Iinclude -gdwarf -Wall -Wextra -O2
LDFLAGS = -m elf_i386

ASM_SRCS = src/asm/start.asm src/asm/trbsyscall.asm
C_SRCS   = src/main.c src/lib/trbstring.c src/lib/trbstdio.c src/lib/trbenv.c src/core/trbshell_exec.c src/core/trbusr.c src/core/builtin/trbbuiltin_exec.c src/core/builtin/trbbuiltin_cmd.c
OBJS     = src/asm/start.o src/asm/trbsyscall.o src/main.o src/lib/trbstring.o src/lib/trbstdio.o src/lib/trbenv.o src/core/trbshell_exec.o src/core/trbusr.o src/core/builtin/trbbuiltin_exec.o src/core/builtin/trbbuiltin_cmd.o

OUTPUT = tortrob_shell

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

%.o: %.c io.h proc.h str.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(OUTPUT)
