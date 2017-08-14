; At start-up, the BIOS:
; * Searches for a boot sector on the bootable devices
;   * A boot sector is a sector ending with a magic word
; * Loads the bootloader from the boot sector into memory at 0x7c00
; * Transfers execution to the bootloader

; The bootloader must then:
; * Find the kernel and load it into memory
; * Shift from real mode to protected mode
; * Prepare the kernel's runtime environment
; * Jump to the kernel

; Our bootloader fits in a single sector, so we don't need a two-stage loader.

[org 0x7c00]                ; Sets the default offset to 0x7c00, as this is   
                            ; the address at which the bootsector is loaded.

mov [BOOT_TYPE], dl         ; At start-up, the boot device's type is stored in 
                            ; `dl`. We store this information for later use.

mov bp, RM_STACK_BASE       ; Sets the base of the real-mode stack.
mov sp, bp                  ; Sets the real-mode stack pointer.

call load_kernel            ; Loads the kernel from disk into memory.
jmp enter_prot_mode         ; Enters protected mode.

[bits 32]                   ; Real mode uses 16-bit instructions, whereas 
                            ; protected mode uses 32-bit instructions.
after_entering_prot_mode:   ; `enter_prot_mode` returns here.
jmp KERNEL_ADDRESS          ; Jumps to where the kernel was loaded.

; CONSTANTS
BOOT_TYPE db 0              ; The boot's drive address (overwritten above).
RM_STACK_BASE equ 0x9000    ; The base of the real-mode stack.
KERNEL_ADDRESS equ 0x1000   ; The address at which the kernel is loaded.
SECTOR_SIZE equ 510         ; The size of a sector excluding the magic word.
MAGIC_WORD equ 0xaa55       ; The magic word identifying a bootsector.

; INCLUDES
; We include the files at the end so that they aren't run unless jumped to.
%include "bootsector/global_descriptor_table.asm"
%include "bootsector/enter_prot_mode.asm"
%include "bootsector/load_kernel.asm"
%include "bootsector/functions/print.asm"
%include "bootsector/functions/disk_load.asm"

; PADDING
times SECTOR_SIZE - ($ - $$) db 0   ; Pads the rest of the bootsector.
dw MAGIC_WORD                       ; A bootsector must end with the magic word.