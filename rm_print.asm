[bits 16]
rm_print:
    ; Saving the contents of the registers.
    pusha

rm_print_body:
    ; Moving the current char into the printing register.
    mov al, [bx]

    ; Ending the function if we have reached the null byte terminating the 
    ; string.
    cmp al, 0
    je rm_print_done

    ; Printing the contents of `al`.
    mov ah, 0x0e
    int 0x10

    ; Incrementing the pointer and looping.
    add bx, 1
    jmp rm_print_body

rm_print_done:
    ; Restoring the contents of the registers.
    popa
    ret