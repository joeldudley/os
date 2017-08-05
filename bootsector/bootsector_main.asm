; At start-up, the BIOS:
; * Searches for a boot sector on the bootable devices
;   * A boot sector is one ending with a magic number
; * Loads the bootloader from the boot sector into memory at 0x7c00
; * Transfers execution to the bootloader

; The bootloader must then:
; * Find and load the kernel into memory
; * Shift from real mode into protected mode
; * Prepare the kernel's runtime environment
; * Jump to the kernel

; Our bootloader fits in a single sector, so we don't need a two-stage loader.

; `db` places the specified byte in the binary.
; `dw` places the two specified bytes (i.e. a word) in the binary.
; `equ` gives a symbolic name to a numeric constant.

[org 0x7c00]                ; Sets the default offset to 0x7c00, as this is   
                            ; the address at which the bootsector is loaded.

mov [BOOT_TYPE], dl         ; The boot device's type is stored in `dl`.
                            ; We store this information for later use.

mov bp, 0x9000              ; Sets the base of the stack.
mov sp, bp                  ; Sets the stack pointer.

call load_kernel            ; Loads the kernel from disk into memory.
jmp enter_prot_mode         ; Enters protected mode.

[bits 32]                   ; Real mode uses 16-bit instructions, whereas 
                            ; protected mode uses 32-bit instructions.
after_entering_prot_mode:   ; `enter_prot_mode` returns here.
jmp KERNEL_ADDRESS          ; Jumps to where the kernel was loaded.

; CONSTANTS
BOOT_TYPE db 0              ; The boot's drive address (overwritten above).
KERNEL_ADDRESS equ 0x1000   ; The address where the kernel is loaded.

; INCLUDES
; We include the files at the end so that they aren't run unless jumped to.
%include "bootsector/global_descriptor_table.asm"
%include "bootsector/enter_prot_mode.asm"
%include "bootsector/helpers/print.asm"
%include "bootsector/helpers/disk_load.asm"
%include "bootsector/load_kernel.asm"

; PADDING
times 510 - ($ - $$) db 0   ; Pads the rest of the boot sector.
dw 0xaa55                   ; Magic word identifying a boot sector.