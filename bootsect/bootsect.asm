; At boot, the BIOS loads our boot code from the first sector of the boot 
; medium into the memory space starting at 0x7c00. The boot medium's type is 
; stored in `dl`.

[org 0x7c00]                ; Sets 0x7c00 as the default offset.

mov [BOOT_DRIVE], dl        ; Stores the boot medium's type for later.

mov bp, 0x9000              ; Initialises the stack.
mov sp, bp                  ;   "

call load_kernel            ; Loads the kernel from disk into memory.
jmp enter_prot_mode         ; Enters protected mode.

[bits 32]
after_entering_prot_mode:   ; `enter_prot_mode` returns here.
    jmp KERNEL_ADDRESS      ; Jumps to where `call_kernel` is loaded.

; CONSTANTS
BOOT_DRIVE db 0             ; The boot's drive address (assigned above).
KERNEL_ADDRESS equ 0x1000   ; The address where the kernel is loaded.

; INCLUDES
; `include` directives are placed here so that they aren't run until required.
%include "bootsect/gdt.asm"
%include "bootsect/enter_prot_mode.asm"
%include "bootsect/helpers/print.asm"
%include "bootsect/helpers/load_from_disk.asm"
%include "bootsect/load_kernel.asm"

; PADDING
times 510 - ($ - $$) db 0   ; Pads the rest of the boot sector.
dw 0xaa55                   ; Magic word identifying a boot sector.