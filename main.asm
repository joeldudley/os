[org 0x7c00]                        ; Origin address at which NASM assumes the program begins.
mov bp, 0x9000                      ; Initialising the stack.
mov sp, bp                          ;   "

mov bx, REAL_MODE_MSG               ; Printing the real-mode message.
call rm_print                       ;   "

call enter_prot_mode                ; Switching to protected mode.
call CODE_SEG:init_prot_mode        ; A far-jump to flush the CPU pipeline.

[bits 32]
prot_mode                           ; After intialising protected mode, we jump to here.

mov ebx, PROT_MODE_MSG              ; Printing the protected-mode message.
call pm_print                       ;   "

jmp $                               ; Loop forever.

REAL_MODE_MSG:
    db 'Entered real mode.', 0      ; Strings must be null-terminated.
PROT_MODE_MSG:
    db "Entered protected mode.", 0

%include "gdt.asm"                  ; If the `include` directives come before 
%include "enter_prot_mode.asm"      ; the infinite loop, their code will be 
%include "init_prot_mode.asm"       ; run as they are included.
%include "rm_print.asm"             ;   "
%include "pm_print.asm"             ;   "

times 510 - ($ - $$) db 0           ; Padding the rest of the boot sector.
dw 0xaa55                           ; Magic word identifying the sector as a boot sector.