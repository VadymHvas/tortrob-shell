global trb_write
global trb_read
global trb_execve
global trb_waitpid
global trb_open
global trb_access
global trb_getcwd
global trb_close
global trb_exit
global trb_fork
global trb_getuid
global trb_chdir
global trb_errno

section .text

; ---------------------------
; Generic syscall wrapper for 3 arguments
; Arguments are passed via stack: 
; [ebp+8] = arg1, [ebp+12] = arg2, [ebp+16] = arg3
; Returns syscall result in eax or -1 on error (errno stored)
; ---------------------------
syscall_3:
    push ebp
    mov ebp, esp
    push ebx

    mov ebx, [ebp+8]     ; arg1
    mov ecx, [ebp+12]    ; arg2
    mov edx, [ebp+16]    ; arg3
    int 0x80             ; syscall interrupt

    ; Check for error (eax in range 0xFFFFF000..0xFFFFFFFF)
    mov edx, eax
    and edx, 0FFFFF000h
    cmp edx, 0FFFFF000h
    jne .ok

    mov [trb_errno], eax ; store errno
    mov eax, -1          ; return -1 on error

.ok:
    pop ebx
    mov esp, ebp
    pop ebp
    ret

; ---------------------------
; Generic syscall wrapper for 2 arguments
; [ebp+8] = arg1, [ebp+12] = arg2
; ---------------------------
syscall_2:
    push ebp
    mov ebp, esp
    push ebx

    mov ebx, [ebp+8]     ; arg1
    mov ecx, [ebp+12]    ; arg2
    int 0x80

    mov edx, eax
    and edx, 0FFFFF000h
    cmp edx, 0FFFFF000h
    jne .ok

    mov [trb_errno], eax
    mov eax, -1

.ok:
    pop ebx
    mov esp, ebp
    pop ebp
    ret

; ---------------------------
; Generic syscall wrapper for 1 argument
; [ebp+8] = arg1
; ---------------------------
syscall_1:
    push ebp
    mov ebp, esp
    push ebx

    mov ebx, [ebp+8]     ; arg1
    int 0x80

    mov edx, eax
    and edx, 0FFFFF000h
    cmp edx, 0FFFFF000h
    jne .ok

    mov [trb_errno], eax
    mov eax, -1

.ok:
    pop ebx
    mov esp, ebp
    pop ebp
    ret

; ---------------------------
; Generic syscall wrapper for no arguments
; Returns syscall result in eax or -1 on error
; ---------------------------
syscall_void:
    int 0x80

    mov edx, eax
    and edx, 0FFFFF000h
    cmp edx, 0FFFFF000h
    jne .ok

    mov [trb_errno], eax
    mov eax, -1

.ok:
    ret

; ---------------------------
; Syscall wrappers with syscall numbers
; Each jumps to corresponding generic syscall wrapper
; ---------------------------

trb_write:
    mov eax, 4          ; sys_write
    jmp syscall_3

trb_read:
    mov eax, 3          ; sys_read
    jmp syscall_3

trb_execve:
    mov eax, 11         ; sys_execve
    jmp syscall_3

trb_waitpid:
    mov eax, 7          ; sys_waitpid
    jmp syscall_3

trb_open:
    mov eax, 5          ; sys_open
    jmp syscall_3

trb_access:
    mov eax, 33         ; sys_access
    jmp syscall_2

trb_getcwd:
    mov eax, 183        ; sys_getcwd
    jmp syscall_2

trb_close:
    mov eax, 6          ; sys_close
    jmp syscall_1

trb_exit:
    mov eax, 1          ; sys_exit
    jmp syscall_1

trb_chdir:
    mov eax, 12         ; sys_chdir
    jmp syscall_1

trb_fork:
    mov eax, 2          ; sys_fork
    jmp syscall_void

trb_getuid:
    mov eax, 24         ; sys_getuid
    jmp syscall_void

section .bss
    trb_errno resd 1    ; storage for errno
