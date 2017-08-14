[bits 16]
print:                 ; Prints the string starting at `bx`.
    pusha

print_body:            ; Prints each char of the string.
    mov al, [bx]       ; Sets `al` to the string's next char.

    cmp al, 0          ; Ends the function if we have reached the null byte terminating the string.
    je print_done      ;   "

    mov ah, TELETYPE   ; Sets 'teletype output' mode for BIOS interrupt 0x10 (video services).
    int VIDEO          ; Prints the char in `al` to the screen.

    add bx, 1          ; Increments the char pointer and loops.
    jmp print_body     ;   "

print_done:            ; Prints a new line.
    mov al, NEW_LINE_L ; The new-line char.
    mov ah, NEW_LINE_H ;   "
    int VIDEO
    mov al, CAR_RTRN_L ; The carriage return char.
    mov ah, CAR_RTRN_H ;   "
    int VIDEO

    popa
    ret

; CONSTANTS
TELETYPE equ 0x0e      ; 'teletype output' mode for BIOS interrupt 0x10 (video services).
VIDEO equ 0x10         ; The BIOS interrupt for video services.
NEW_LINE_L equ 0x0a    ; The lower part of the new-line char.
NEW_LINE_H equ 0x0e    ; The higher part of the new-line char.
CAR_RTRN_L equ 0x0d    ; The lower part of the carriage return char.
CAR_RTRN_H equ 0x0e    ; The higher part of the carriage return char.