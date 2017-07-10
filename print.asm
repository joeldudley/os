print:
    ; Saving the contents of the registers.
    pusha

body:
    ; Moving the current char into the printing register.
    mov al, [bx]

    ; Ending the function if we have reached the null byte terminating the 
    ; string.
    cmp al, 0
    je end

    ; Printing the contents of `al`.
    mov ah, 0x0e
    int 0x10

    ; Incrementing the pointer and looping.
    add bx, 1
    jmp body

end:
    ; Restoring the contents of the registers.
    popa
    ret