[bits 16]
enter_prot_mode:                    ; Switches to protected mode.
    cli                             ; Disable interrupts.
    lgdt [gdt_descriptor]           ; Load the GDT descriptor.
    mov eax, cr0                    ; Set the first bit of cr0.
    or eax, 0x1                     ;   "
    mov cr0, eax                    ;   "
    call CODE_SEG:init_prot_mode    ; A far-jump flushing the CPU pipeline and initialising protected mode.

[bits 32]
init_prot_mode:
    mov ax, DATA_SEG                ; Update our segment registers to point to the data segment.
    mov ds, ax                      ;   "
    mov ss, ax                      ;   "
    mov es, ax                      ;   "
    mov fs, ax                      ;   "
    mov gs, ax                      ;   "

    mov ebp, 0x90000                ; Update the stack right to the top of the free space.
    mov esp, ebp                    ;   "

    jmp after_entering_prot_mode    ; I believe we can't just use `ret` here because we've messed with the registers so much.