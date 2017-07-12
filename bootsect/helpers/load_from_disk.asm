[bits 16]
disk_load:          ; Loads 'dh' sectors from medium `dl` into `es:bx`.
                    ; Where for `dl`, 0 = floppy, 1 = floppy2, 0x80 = hdd, and 0x81 = hdd2.
    pusha
    push dx         ; This function will also overwrite 'dx'.

    mov ah, 0x02    ; Sets BIOS interrupt 0x13 (disk read/write) to 0x02 ('read').
    mov al, dh      ; Sets `al` to the number of sectors to read.
    mov cl, 0x02    ; Sets `cl` to the sector to read (0x01 is boot).
    mov ch, 0x00    ; Sets `ch` to the cylinder to read.
    mov dh, 0x00    ; Sets `dh` to the head to read.

    int 0x13        ; Reads from disk, and ends if the read failed.
    jc disk_err     ;   "

    pop dx
    cmp al, dh      ; Checks if we have read the right number of sectors, and ends otherwise.
    jne sectors_err ;   "
    popa
    ret

disk_error:
    DISK_ERROR: db "Disk read error", 0
    mov bx, DISK_ERROR
    call print
    jmp $

sectors_error:
    SECTORS_ERROR: db "Incorrect number of sectors read", 0
    mov bx, SECTORS_ERROR
    call print
    jmp $