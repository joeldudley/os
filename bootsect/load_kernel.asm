[bits 16]
load_kernel:                ; Reads sectors 1-17 (our kernel) from the disk and stores them in memory.
    mov bx, KERNEL_ADDRESS
    mov dh, 16
    mov dl, [BOOT_DRIVE]
    call disk_load 
    ret