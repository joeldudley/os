[bits 16]
load_kernel:                ; Reads our kernel from sector 2 onwards of the boot medium and stores 
                            ; it in memory.
    mov bx, KERNEL_ADDRESS  ; The address our kernel will be loaded at.
    mov dh, KERNAL_SIZE     ; The size of our kernel in sectors.
    mov dl, [BOOT_TYPE]     ; The medium the kernel is being loaded from.
    call disk_load 
    ret

; CONSTANTS
KERNAL_SIZE equ 32          ; The size of our kernel in sectors.