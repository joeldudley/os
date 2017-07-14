[bits 16]
load_kernel:                ; Reads our kernel from the boot medium and stores it in memory.
    mov bx, KERNEL_ADDRESS
    mov dh, 16              ; Our kernel ocuupies sectors 1-17 (i.e. 16 sectors).
    mov dl, [BOOT_DRIVE]
    call disk_load 
    ret