[org 0x7c00]                            ; Origin address at which NASM assumes the program begins.
mov bx, REAL_MODE_MSG                   ; Printing the real-mode message.
call rm_print                           ;   "

mov [BOOT_DRIVE], dl                    ; We store the boot drive's address.
mov bp, 0x9000                          ; Initialising the stack.
mov sp, bp                              ;   "

call load_kernel                        ; Loading the kernel.
mov bx, KERNEL_LOAD_MSG
call rm_print

call enter_prot_mode                    ; Entering protected mode.

[bits 32]
after_entering_prot_mode:               ; `enter_prot_mode` returns to here.
    mov ebx, PROT_MODE_MSG              ; Printing the protected-mode message.
    call pm_print                       ;   "
    ; call KERNEL_OFFSET
    jmp $

REAL_MODE_MSG:
    db 'Entered real mode.', 0          ; Strings must be null-terminated.
KERNEL_LOAD_MSG:
    db "Loaded kernel into memory.", 0
PROT_MODE_MSG:
    db "Entered protected mode.", 0

KERNEL_OFFSET equ 0x1000
BOOT_DRIVE db 0                         ; Storing the boot drive's address (? is that right) in case it's overwritten.

%include "bootsect/gdt.asm"             ; If the `include` directives come before 
%include "bootsect/enter_prot_mode.asm" ; the infinite loop, their code will be 
%include "bootsect/rm_print.asm"        ; run as they are included.
%include "bootsect/pm_print.asm"        ;   "
%include "bootsect/load_from_disk.asm"  ;   "
%include "bootsect/load_kernel.asm"     ;   "

times 510 - ($ - $$) db 0               ; Padding the rest of the boot sector.
dw 0xaa55                               ; Magic word identifying the sector as a boot sector.