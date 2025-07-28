global _start          ; Entry point
extern main            ; main function in C

section .text

_start:
    mov ecx, [esp]        ; argc
    lea eax, [esp+4]      ; argv
    mov edx, eax          ; save argv

    mov ebx, ecx
    shl ebx, 2            ; argc * 4 (size of argv)
    add ebx, 4            ; +4 for NULL after argv
    add ebx, eax          ; envp = argv + argc + 1

    push ebx              ; envp
    push eax              ; argv
    push ecx              ; argc

    call main             ; call main(argc, argv, envp)

    add esp, 8            ; clean up stack (2 args * 4 bytes)

    mov ebx, eax          
    mov eax, 1
    int 0x80
