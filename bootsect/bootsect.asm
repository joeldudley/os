[org 0x7c00]                            ; Origin address at which NASM assumes the program begins.
BOOT_DRIVE db 0                         ; Storing the boot drive's address in case it's overwritten.
mov [BOOT_DRIVE], dl                    ; We store the boot drive's address.
mov bp, 0x9000                          ; Initialising the stack.
mov sp, bp                              ;   "

call load_kernel                        ; Loading the kernel.
call enter_prot_mode                    ; Entering protected mode.

[bits 32]
after_entering_prot_mode:               ; `enter_prot_mode` returns to here.
    call KERNEL_OFFSET
    jmp $

KERNEL_OFFSET equ 0x1000

%include "bootsect/gdt.asm"             ; If the `include` directives come before 
%include "bootsect/enter_prot_mode.asm" ; the infinite loop, their code will be 
%include "bootsect/print.asm"           ; run as they are included.
%include "bootsect/load_from_disk.asm"  ; run as they are included.
%include "bootsect/load_kernel.asm"     ;   "

times 510 - ($ - $$) db 0               ; Padding the rest of the boot sector.
dw 0xaa55                               ; Magic word identifying the sector as a boot sector.