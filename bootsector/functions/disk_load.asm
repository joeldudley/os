[bits 16]
disk_load:           ; Read 'dh' sectors from sector 2 onwards of the medium of type `dl` into 
                     ; `es:bx`.
    pusha            ; `pusha` pushes all the general purpose registers onto the stack.
    push dx          ; This function will overwrite 'dx'.

    mov ah, READ     ; Sets `read` mode for BIOS interrupt 0x13 (disk read/write).
    mov al, dh       ; The number of sectors to read.
    mov cl, SECTOR   ; The sector, cylinder and head to start reading from.
    mov ch, CYLINDER ;   "
    mov dh, HEAD     ;   "

    int DISK_RW      ; Reads from disk into `es:bx`, and throws an error if the read failed.
    jc disk_err      ;   "

    pop dx
    cmp al, dh       ; Throws an error if we haven't read the right number of sectors.
    jne sectors_err  ;   "
    popa
    ret

; CONSTANTS
READ equ 0x02        ; 'read' mode for BIOS interrupt 0x13 (disk read/write).
SECTOR equ 0x02      ; The sector to start reading from (0x01 is boot).
CYLINDER equ 0x00    ; The cylinder to read.
HEAD equ 0x00        ; The head to read.
DISK_RW equ 0x13     ; The BIOS interrupt for disk read/write.
DISK_ERROR db "Disk read error", 0                      ; Strings are null-byte terminated.
SECTORS_ERROR db "Incorrect number of sectors read", 0  ;   "

disk_err:
    mov bx, DISK_ERROR
    call print
    jmp $

sectors_err:
    mov bx, SECTORS_ERROR
    call print
    jmp $