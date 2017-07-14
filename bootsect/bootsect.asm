; At start-up, the BIOS:
; * Searches for a boot sector on the bootable devices
;   * A boot sector is identified by its magic number
; * Loads the bootloader from the boot sector into memory at 0x7c00
; * Transfers execution to the bootloader
;   * Initially, the bootloader is executing in real mode

; The bootloader must then:
; * Find and load the kernel into memory
; * Enable protected mode
; * Prepare the kernel's runtime environment

; Unless the bootloader fits entirely in the boot sector, we'll need a 
; two-stage loader.

[org 0x7c00]                ; Sets 0x7c00 as the default offset.

mov [BOOT_DRIVE], dl        ; The boot device's type is stored in `dl`.
                            ; We store this information for later use.

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