; `pusha` pushes all the general purpose registers onto the stack.

[bits 16]
disk_load:          ; Read 'dh' sectors from sector 2 onwards of the medium of type `dl` into 
                    ; `es:bx`.
    pusha
    push dx         ; This function will also overwrite 'dx'.

    mov ah, 0x02    ; Sets 0x02 ('read') mode for the call to BIOS interrupt 0x13 (disk read/write).
    mov al, dh      ; The number of sectors to read.
    mov cl, 0x02    ; The sector to read (0x01 is boot).
    mov ch, 0x00    ; The cylinder to read.
    mov dh, 0x00    ; The head to read.

    int 0x13        ; Reads from disk into `es:bx`, and throws an error if the read failed.
    jc disk_err     ;   "

    pop dx
    cmp al, dh      ; Throws an error if we haven't read the right number of sectors.
    jne sectors_err ;   "
    popa
    ret

disk_err:
    DISK_ERROR: db "Disk read error", 0
    mov bx, DISK_ERROR
    call print
    jmp $

sectors_err:
    SECTORS_ERROR: db "Incorrect number of sectors read", 0
    mov bx, SECTORS_ERROR
    call print
    jmp $