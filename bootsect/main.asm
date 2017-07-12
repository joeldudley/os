[org 0x7c00]                ; Origin address at which NASM assumes the program begins.
mov [BOOT_DRIVE], dl        ; Stores the boot drive's address in case it's overwritten.

mov bp, 0x9000              ; Initialises the stack.
mov sp, bp                  ;   "

call load_kernel            ; Loads the kernel.
call enter_prot_mode        ; Enters protected mode.
after_entering_prot_mode    ; `enter_prot_mode` returns here.

[bits 32]
call KERNEL_ADDRESS         ; Jumps to where the kernel is loaded.

; CONSTANTS
BOOT_DRIVE db 0             ; The boot's drive address (updated above).
KERNEL_ADDRESS equ 0x1000   ; The address where the kernel is loaded.

; INCLUDES
; Put `include` directives at the end so they aren't run until required.
%include "bootsect/gdt.asm"
%include "bootsect/enter_prot_mode.asm"
%include "bootsect/helpers/print.asm"
%include "bootsect/helpers/load_from_disk.asm"
%include "bootsect/load_kernel.asm"

; PADDING
times 510 - ($ - $$) db 0   ; Pads the rest of the boot sector.
dw 0xaa55                   ; Magic word identifying the sector as a boot sector.