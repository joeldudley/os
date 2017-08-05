[bits 16]
gdt_start:
    dd 0x0                          ; The GDT starts with 8 null bytes.
    dd 0x0                          ;   "

gdt_code:                           ; GDT for the code segment.
; Base = 0x0, limit = 0xfffff.
; 1st set of flags: present = 1, privilege = 00, descriptor type = 1.
; Type flags: code = 1, conforming = 0, readable = 1, accessed = 0.
; 2nd set of flags: granularity = 1, 32-bit default = 1, 64-bit seg = 0, AVL = 0.
    dw LIMIT_0_15                      
    dw BASE_0_15                        
    db BASE_16_23 
    db _1ST_N_TYPE_FLGS_CODE
    db _2ND_FLGS_N_LMT_16_19                    
    db BASE_24_31  

gdt_data:                           ; GDT for the data segment.
; As above, except type flags.
; Type flags: code = 0, expand down = 0, writable = 1, accessed = 0.
    dw LIMIT_0_15                      
    dw BASE_0_15                        
    db BASE_16_23                          
    db _1ST_N_TYPE_FLGS_DATA          
    db _2ND_FLGS_N_LMT_16_19                    
    db BASE_24_31       

gdt_descriptor:                     ; GDT descriptor.
    dw $ - gdt_start - 1            ; The GDT's 16-bit size, equal to one less than its true size.
    dd gdt_start                    ; The GDT's 32-bit address.

; CONSTANTS
LIMIT_0_15 equ 0xfffff              ; Limit, bits 0-15.
BASE_0_15 equ 0x0                   ; Base, bits 0-15.
BASE_16_23 equ 0x0                  ; Base, bits 16-23.
BASE_24_31 equ 0x0                  ; Base, bits 24-31.
_1ST_N_TYPE_FLGS_CODE equ 10011010b ; First set of flags and type flags for the data segment.
_1ST_N_TYPE_FLGS_DATA equ 10010010b ; First set of flags and type flags for the code segment.
_2ND_FLGS_N_LMT_16_19 equ 11001111b ; Second set of flags and limit (bits 16-19).
CODE_SEG equ gdt_code - gdt_start   ; The size of the code segment.
DATA_SEG equ gdt_data - gdt_start   ; The size of the data segment.