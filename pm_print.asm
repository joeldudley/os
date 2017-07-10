[bits 32]

VIDEO_MEMORY equ 0xb8000    ; Defining a constant.
WHITE_ON_BLACK equ 0x0f     ; The color byte for each character.

pm_print:
    pusha
    mov edx, VIDEO_MEMORY

pm_print_body:
    mov al, [ebx]           ; [ebx] is the address of our character
    mov ah, WHITE_ON_BLACK

    cmp al, 0               ; check if end of string
    je pm_print_done

    mov [edx], ax           ; store character + attribute in video memory
    add ebx, 1              ; next char
    add edx, 2              ; next video memory position

    jmp pm_print_body

pm_print_done:
    popa
    ret
