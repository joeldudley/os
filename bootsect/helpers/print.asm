[bits 16]
; WHAT IS BX?
print:
    pusha

print_body:         ; Prints each char of the string.
    mov al, [bx]    ; Moves the string's next char into register `al`.

    cmp al, 0       ; Ends the function if we have reached the null byte terminating the string.
    je print_done   ;   "

    mov ah, 0x0e    ; Sets BIOS interrupt 0x13 (video services) to 0x0e ('teletype output').
    int 0x10        ; Calls BIOS interrupt 0x10, printing the char in `al` to screen.

    add bx, 1       ; Increments the char pointer and loops.
    jmp print_body  ;   "

print_done:         ; Prints a new line.
    mov al, 0x0a    ; The new-line char.
    mov ah, 0x0e  
    int 0x10
    mov al, 0x0d    ; The carriage return char.
    mov ah, 0x0e
    int 0x10

    ; Returning.
    popa
    ret