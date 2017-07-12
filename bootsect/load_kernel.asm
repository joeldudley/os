[bits 16]
load_kernel: ; Reads 16 sectors from the boot drive and stores it at the kernel address.
    mov bx, KERNEL_ADDRESS
    mov dh, 16
    mov dl, [BOOT_DRIVE]
    call disk_load 
    ret