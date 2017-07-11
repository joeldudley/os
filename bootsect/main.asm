[org 0x7c00]                ; Origin address at which NASM assumes the program begins.
mov [BOOT_DRIVE], dl        ; Storing the boot drive's address in case it's overwritten.

mov bp, 0x9000              ; Initialising the stack.
mov sp, bp                  ;   "

call load_kernel            ; Loading the kernel.
call enter_prot_mode        ; Entering protected mode.
after_entering_prot_mode    ; `enter_prot_mode` returns to here.

[bits 32]
call KERNEL_ADDRESS         ; Jump to where the kernel is loaded.

; CONSTANTS
BOOT_DRIVE db 0             ; Used when loading the kernel.
KERNEL_ADDRESS equ 0x1000   ; Used when loading and entering the kernel.

; INCLUDES
; We put `include` directives at the end so they aren't run until required.
%include "bootsect/gdt.asm"
%include "bootsect/enter_prot_mode.asm"
%include "bootsect/helpers/print.asm"
%include "bootsect/helpers/load_from_disk.asm"
%include "bootsect/load_kernel.asm"

; PADDING
times 510 - ($ - $$) db 0   ; Padding the rest of the boot sector.
dw 0xaa55                   ; Magic word identifying the sector as a boot sector.