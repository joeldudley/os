; The Global Descriptor Table is a data structure that uses "descriptors" to define the 
; characteristics of the various memory areas used during program execution, including their base 
; address, size, and access privileges such as executability and writability.
; Each descriptor is 8-bytes long.
[bits 16]
gdt_start:
    dd 0x0                          ; The GDT starts with 8 null bytes.
    dd 0x0                          ;   "

gdt_code:                           ; The descriptor for the code segment.
                                    ; `cs` should point to this descriptor.
; 1st set of flags: present, privilege, descriptor type.
; 2nd set of flags: granularity, 32-bit default, 64-bit seg, AVL.
; Type flags: code, conforming, readable, accessed.
    dw LIMIT_0_15                   ; Limit, bits 0-15.
    dw BASE_0_15                    ; Base, bits 0-15.
    db BASE_16_23                   ; Base, bits 16-23.
    db _1ST_N_TYPE_FLGS_CODE        ; First set of flags and type flags.
    db _2ND_FLGS_N_LMT_16_19        ; Second set of flags and limit, bits 16-19.
    db BASE_24_31                   ; Base, bits 24-31.

gdt_data:                           ; The descriptor for the data segment.
                                    ; `ds`, `ss`, `es`, `fs`, and `gs` should point to this descriptor.
; As above, except for the type flags.
; Type flags: code, expand down, writable, accessed.
    dw LIMIT_0_15                   ; Limit, bits 0-15.
    dw BASE_0_15                    ; Base, bits 0-15.
    db BASE_16_23                   ; Base, bits 16-23.
    db _1ST_N_TYPE_FLGS_DATA        ; First set of flags and type flags.
    db _2ND_FLGS_N_LMT_16_19        ; Second set of flags and limit, bits 16-19.
    db BASE_24_31                   ; Base, bits 24-31. 

gdt_descriptor:                     ; GDT descriptor.
    dw $ - gdt_start - 1            ; The GDT's 16-bit size, equal to one less than its true size.
    dd gdt_start                    ; The GDT's 32-bit address.

; CONSTANTS
LIMIT_0_15 equ 0xfffff              ; Limit = 0xfffff.
BASE_0_15 equ 0x0                   ; Base = 0x0.
BASE_16_23 equ 0x0                  ;   "
BASE_24_31 equ 0x0                  ;   "
_1ST_N_TYPE_FLGS_CODE equ 10011010b ; Present = 1, privilege = 00, descriptor type = 1, 
                                    ; code = 1, conforming = 0, readable = 1, accessed = 0.
_1ST_N_TYPE_FLGS_DATA equ 10010010b ; Present = 1, privilege = 00, descriptor type = 1, 
                                    ; code = 0, expand down = 0, writable = 1, accessed = 0.
_2ND_FLGS_N_LMT_16_19 equ 11001111b ; Granularity = 1, 32-bit default = 1, 64-bit seg = 0, AVL = 0.
CODE_SEG equ gdt_code - gdt_start   ; The index of the code segment in the GDT.
DATA_SEG equ gdt_data - gdt_start   ; The index of the data segment in the GDT.