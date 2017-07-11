[bits 16]
print:
    pusha

print_body:
    ; Moving the current char into the printing register.
    mov al, [bx]

    ; Ending the function if we have reached the null byte terminating the string.
    cmp al, 0
    je print_done

    ; Printing the contents of `al`.
    mov ah, 0x0e
    int 0x10

    ; Incrementing the pointer and looping.
    add bx, 1
    jmp print_body

print_done:
    ; Printing a new-line character.
    mov al, 0x0a
    mov ah, 0x0e
    int 0x10
    mov al, 0x0d
    mov ah, 0x0e
    int 0x10

    ; Returning.
    popa
    ret