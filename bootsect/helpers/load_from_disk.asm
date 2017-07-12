[bits 16]
disk_load:          ; Loads 'dh' sectors from drive 'dl' into ES:BX.
    pusha
    push dx         ; This function will also overwrite 'dx'.

    mov ah, 0x02    ; Sets BIOS interrupt 0x13 (disk read/write) to 0x02 ('read').
    mov al, dh      ; al <- number of sectors to read (0x01 .. 0x80)
    mov cl, 0x02    ; cl <- sector (0x01 .. 0x11)
                 ; 0x01 is our boot sector, 0x02 is the first 'available' sector
    mov ch, 0x00 ; ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')
    ; dl <- drive number. Our caller sets it as a parameter and gets it from BIOS
    ; (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
    mov dh, 0x00 ; dh <- head number (0x0 .. 0xF)

    ; [es:bx] <- pointer to buffer where the data will be stored
    ; caller sets it up for us, and it is actually the standard location for int 13h
    int 0x13      ; BIOS interrupt
    jc disk_error ; if error (stored in the carry bit)

    pop dx
    cmp al, dh    ; BIOS also sets 'al' to the # of sectors read. Compare it.
    jne sectors_error
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