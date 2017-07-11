[bits 16]
load_kernel: 
    mov bx, KERNEL_ADDRESS   ; Read from disk and store in 0x1000
    mov dh, 2
    mov dl, [BOOT_DRIVE]
    call disk_load 
    ret