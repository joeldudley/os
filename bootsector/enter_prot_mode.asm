[bits 16]
enter_prot_mode:                    ; Enters protected mode.
    cli                             ; Disables interrupts.
    lgdt [gdt_descriptor]           ; Loads the global table descriptor.
    mov eax, cr0                    ; Set the first bit of cr0. ??? WHY? ???
    or eax, 0x1                     ;   "
    mov cr0, eax                    ;   "
    jmp CODE_SEG:init_prot_mode     ; A far-jump flushing the CPU pipeline.

[bits 32]
init_prot_mode:                     ; Initialises protected mode.
    mov ax, DATA_SEG                ; Update our segment registers to point to the data segment.
    mov ds, ax                      ;   "
    mov ss, ax                      ;   "
    mov es, ax                      ;   "
    mov fs, ax                      ;   "
    mov gs, ax                      ;   "

    mov ebp, PM_STACK_BASE          ; Updates the stack to point to the top of the free space.
    mov esp, ebp                    ;   "

    jmp after_entering_prot_mode    ; We can't use `ret` here because we've messed with the 
                                    ; registers so much.

; CONSTANTS
PM_STACK_BASE equ 0x90000           ; The base of the protected-mode stack.