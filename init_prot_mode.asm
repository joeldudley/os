[bits 32]
init_prot_mode:
    mov ax, DATA_SEG    ; Update our segment registers to point to the data segment.
    mov ds, ax          ;   "
    mov ss, ax          ;   "
    mov es, ax          ;   "
    mov fs, ax          ;   "
    mov gs, ax          ;   "

    mov ebp, 0x90000    ; Update the stack right to the top of the free space.
    mov esp, ebp        ;   "

    jmp prot_mode       ; I believe we can't just use `ret` here because we've messed with the registers so much.