; Origin address at which NASM assumes the program begins.
[org 0x7c00]

; Printing a string.
mov bx, MSG
call print

; Infinite loop.
jmp $

; Including the `print_char` function.
%include "print.asm"

; The string we'll print.
MSG:
    ; The string is null-terminated.
    db 'qwerty', 0

; Padding the rest of the boot sector.
times 510 - ($ - $$) db 0

; Magic word identifying the sector as a boot sector.
dw 0xaa55