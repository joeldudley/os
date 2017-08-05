[bits 16]
gdt_start:
    dd 0x0                          ; The GDT starts with 8 null bytes.
    dd 0x0                          ;   "

gdt_code:                           ; GDT for the code segment.
; Base = 0x0, limit = 0xfffff.
; 1st set of flags: present = 1, privilege = 00, descriptor type = 1.
; Type flags: code = 1, conforming = 0, readable = 1, accessed = 0.
; 2nd set of flags: granularity = 1, 32-bit default = 1, 64-bit seg = 0, AVL = 0.
    dw 0xffff                       ; Limit, bits 0-15.
    dw 0x0                          ; Base, bits 0-15.
    db 0x0                          ; Base, bits 16-23.
    db 10011010b                    ; First set of flags & type flags.
    db 11001111b                    ; Second set of flags & limit (bits 16-19).
    db 0x0                          ; Base, bits 24-31.

gdt_data:                           ; GDT for the data segment.
; Base = 0x0, limit = 0xfffff.
; 1st set of flags: present = 1, privilege = 00, descriptor type = 1.
; Type flags: code = 0, expand down = 0, writable = 1, accessed = 0.
; 2nd set of flags: granularity = 1, 32-bit default = 1, 64-bit seg = 0, AVL = 0.
    dw 0xffff                       ; Limit, bits 0-15.
    dw 0x0                          ; Base, bits 0-15.
    db 0x0                          ; Base, bits 16-23.
    db 10010010b                    ; First set of flags & type flags.
    db 11001111b                    ; Second set of flags & limit (bits 16-19).
    db 0x0                          ; Base, bits 24-31.

gdt_end:                            ; Can I remove this, and replace `gdt_end` with `$`?

gdt_descriptor:                     ; GDT descriptor.
    dw gdt_end - gdt_start - 1      ; Size (16 bit), always one less than its true size.
    dd gdt_start                    ; Address (32 bit)

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start