; At start-up, the BIOS:
; * Searches for a boot sector (i.e. a sector ending with a magic word) on the bootable devices
; * Loads the bootloader from the boot medium into memory at 0x7c00
; * Transfers execution to the bootloader

; For backwards compatibiity, the CPU boots in 16-bit mode.

; The bootloader must then:
; * Find the kernel and load it into memory
; * Shift from real mode to protected mode
; * Prepare the kernel's runtime environment
; * Jump to the kernel

; Our bootloader fits in a single sector, so we don't need a two-stage loader.

; PREP

; Sets the default offset to 0x7c00, the address at which the bootsector is loaded. Allows us to
; address memory as if the bootsector was loaded at 0x0.
[org 0x7c00]
; Sets the base of the real-mode stack and stack pointer.
mov bp, RM_STACK_BASE
mov sp, RM_STACK_BASE

; LOADING THE KERNEL

; Sets `read` mode for BIOS interrupt 0x13 (disk read/write).
mov ah, READ
; The address our kernel will be loaded at.
mov bx, KERNEL_ADDRESS
; The number of sectors to read.
mov al, KERNAL_SIZE
; The sector, cylinder and head to start reading from.
mov cl, SECTOR
mov ch, CYLINDER
mov dh, HEAD
; Reads our kernel from the boot medium and stores it in memory at `es:bx`.
; At start-up, the boot medium's type is stored in `dl`.
int DISK_RW

; ENTERING PROTECTED MODE

; Disables interrupts.
cli
; Loads the global table descriptor.
lgdt [gdt_descriptor]
; We make the switch the protected mode by setting the first bit of control register CR0, without
; modifying the existing bits.
mov eax, cr0
or eax, 0x1
mov cr0, eax
; Far-jumping forces the CPU to flush any remaining 16-bit operations in its pipeline, and updates
; `cs` to point to the code segment of the GDT.
jmp CODE_SEG:init_prot_mode

; INITIALISING PROTECTED MODE

[bits 32]
init_prot_mode:
; Update our segment registers to point to the data segment.
mov ax, DATA_SEG
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax
; Updates the stack to point to the top of the free space.
mov ebp, PM_STACK_BASE
mov esp, ebp

; JUMPING INTO THE KERNEL

jmp KERNEL_ADDRESS

; CONSTANTS

; The base of the real-mode stack.
RM_STACK_BASE equ 0x9000
; 'read' mode for BIOS interrupt 0x13 (disk read/write).
READ equ 0x02
; The address at which the kernel is loaded.
KERNEL_ADDRESS equ 0x1000
; The size of our kernel in sectors.
KERNAL_SIZE equ 31
; The sector to start reading from (0x01 is boot).
SECTOR equ 0x02
; The cylinder to read.
CYLINDER equ 0x00
; The head to read.
HEAD equ 0x00
; The BIOS interrupt for disk read/write.
DISK_RW equ 0x13
; The base of the protected-mode stack.
PM_STACK_BASE equ 0x90000
; The size of a sector excluding the magic word.
SECTOR_SIZE equ 510
; The magic word identifying a bootsector.
MAGIC_WORD equ 0xaa55

; INCLUDES

; We include the files at the end so that they aren't run unless jumped to.
%include "bootsector/global_descriptor_table.asm"

; PADDING

; Pads the rest of the bootsector.
times SECTOR_SIZE - ($ - $$) db 0
; A bootsector must end with the magic word.
dw MAGIC_WORD