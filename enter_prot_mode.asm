[bits 16]
enter_prot_mode:            ; Switches to protected mode.
    cli                     ; Disable interrupts.
    lgdt [gdt_descriptor]   ; Load the GDT descriptor.
    mov eax, cr0            ; Set the first bit of cr0.
    or eax, 0x1             ;   "
    mov cr0, eax            ;   "
    ret