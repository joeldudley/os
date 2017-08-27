[extern handle_isr]
[extern handle_irq]

; Prepares the CPU to handle an interrupt.
prep_isr:
    ; Saves the CPU's current state.
	pusha
	; Lower 16-bits of eax = ds.
	mov ax, ds
	; Save the data segment descriptor.
	push eax
	; Kernel data segment descriptor.
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
    push esp ; interrupt_args_t *r

    ; Handle the ISR.
    ; C code following the sysV ABI requires DF to be clear on function entry
    cld
	call handle_isr

	; Restores the CPU's state.
	pop eax
	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa
	; Cleans up the pushed error code and pushed interrupt number.
	add esp, 8
	; Return from the handling of the interrupt.
	; Pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP.
	iret

; As above, except for the code to restore the CPU's state.
prep_irq_handler:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp

    cld
    call handle_irq

    ; Different to the ISR code. TODO: Find out why.
    pop ebx
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    iret

; INTERRUPT HANDLERS

; We don't get information about which interrupt was called when the handler is run, so we need to
; create a different .asm handler for each interrupt.

; Each .asm interrupt handler pushes the following onto the stack:
; * A dummy error code onto the stack if the interrupt doesn't provide one (some of the
;   CPU-dedicated interrupts do)
; * The interrupt's index in the array of interrupt handler functions

; Special CPU-dedicated interrupt handlers.

; 0: Divide by zero exception.
global isr0_handler
isr0_handler:
    push byte 0
    push byte 0
    jmp prep_isr

; 1: Debug exception.
global isr1_handler
isr1_handler:
    push byte 0
    push byte 1
    jmp prep_isr

; 2: Non maskable interrupt exception.
global isr2_handler
isr2_handler:
    push byte 0
    push byte 2
    jmp prep_isr

; 3: Int 3 exception
global isr3_handler
isr3_handler:
    push byte 0
    push byte 3
    jmp prep_isr

; 4: INTO exception.
global isr4_handler
isr4_handler:
    push byte 0
    push byte 4
    jmp prep_isr

; 5: Out of bounds exception.
global isr5_handler
isr5_handler:
    push byte 0
    push byte 5
    jmp prep_isr

; 6: Invalid opcode exception.
global isr6_handler
isr6_handler:
    push byte 0
    push byte 6
    jmp prep_isr

; 7: Coprocessor not available exception.
global isr7_handler
isr7_handler:
    push byte 0
    push byte 7
    jmp prep_isr

; 8: Double fault exception.
global isr8_handler
isr8_handler:
    ; Pushes an error code onto the stack.
    push byte 8
    jmp prep_isr

; 9: Coprocessor segment overrun exception.
global isr9_handler
isr9_handler:
    push byte 0
    push byte 9
    jmp prep_isr

; 10: Bad TSS exception.
global isr10_handler
isr10_handler:
    ; Pushes an error code onto the stack.
    push byte 10
    jmp prep_isr

; 11: Segment not present exception.
global isr11_handler
isr11_handler:
    ; Pushes an error code onto the stack.
    push byte 11
    jmp prep_isr

; 12: Stack fault exception.
global isr12_handler
isr12_handler:
    ; Pushes an error code onto the stack.
    push byte 12
    jmp prep_isr

; 13: General protection fault exception.
global isr13_handler
isr13_handler:
    ; Pushes an error code onto the stack.
    push byte 13
    jmp prep_isr

; 14: Page fault exception.
global isr14_handler
isr14_handler:
    ; Pushes an error code onto the stack.
    push byte 14
    jmp prep_isr

; 15: Reserved exception.
global isr15_handler
isr15_handler:
    push byte 0
    push byte 15
    jmp prep_isr

; 16: Floating point exception.
global isr16_handler
isr16_handler:
    push byte 0
    push byte 16
    jmp prep_isr

; 17: Alignment check exception.
global isr17_handler
isr17_handler:
    push byte 0
    push byte 17
    jmp prep_isr

; 18: Machine check exception.
global isr18_handler
isr18_handler:
    push byte 0
    push byte 18
    jmp prep_isr

; 19: Reserved.
global isr19_handler
isr19_handler:
    push byte 0
    push byte 19
    jmp prep_isr

; 20: Reserved.
global isr20_handler
isr20_handler:
    push byte 0
    push byte 20
    jmp prep_isr

; 21: Reserved.
global isr21_handler
isr21_handler:
    push byte 0
    push byte 21
    jmp prep_isr

; 22: Reserved.
global isr22_handler
isr22_handler:
    push byte 0
    push byte 22
    jmp prep_isr

; 23: Reserved.
global isr23_handler
isr23_handler:
    push byte 0
    push byte 23
    jmp prep_isr

; 24: Reserved.
global isr24_handler
isr24_handler:
    push byte 0
    push byte 24
    jmp prep_isr

; 25: Reserved.
global isr25_handler
isr25_handler:
    push byte 0
    push byte 25
    jmp prep_isr

; 26: Reserved.
global isr26_handler
isr26_handler:
    push byte 0
    push byte 26
    jmp prep_isr

; 27: Reserved.
global isr27_handler
isr27_handler:
    push byte 0
    push byte 27
    jmp prep_isr

; 28: Reserved.
global isr28_handler
isr28_handler:
    push byte 0
    push byte 28
    jmp prep_isr

; 29: Reserved.
global isr29_handler
isr29_handler:
    push byte 0
    push byte 29
    jmp prep_isr

; 30: Reserved.
global isr30_handler
isr30_handler:
    push byte 0
    push byte 30
    jmp prep_isr

; 31: Reserved.
global isr31_handler
isr31_handler:
    push byte 0
    push byte 31
    jmp prep_isr

; Hardware interrupt handlers.

; 0: System timer.
global irq0_handler
irq0_handler:
	cli
	push byte 0
	push byte 32
	jmp prep_irq_handler

; 1: Keyboard controller.
global irq1_handler
irq1_handler:
	cli
	push byte 1
	push byte 33
	jmp prep_irq_handler

; 2: Cascaded signals from IRQs 8-15.
global irq2_handler
irq2_handler:
	cli
	push byte 2
	push byte 34
	jmp prep_irq_handler

; 3: Serial port controller for serial port 2.
global irq3_handler
irq3_handler:
	cli
	push byte 3
	push byte 35
	jmp prep_irq_handler

; 4: Serial port controller for serial port 1.
global irq4_handler
irq4_handler:
	cli
	push byte 4
	push byte 36
	jmp prep_irq_handler

; 5: Parallel port 2 and 3 or sound card.
global irq5_handler
irq5_handler:
	cli
	push byte 5
	push byte 37
	jmp prep_irq_handler

; 6: Floppy disk controller.
global irq6_handler
irq6_handler:
	cli
	push byte 6
	push byte 38
	jmp prep_irq_handler

; 7: Parallel port 1.
global irq7_handler
irq7_handler:
	cli
	push byte 7
	push byte 39
	jmp prep_irq_handler

; 8: Real-time clock.
global irq8_handler
irq8_handler:
	cli
	push byte 8
	push byte 40
	jmp prep_irq_handler

; 9: Advanced Configuration and Power Interface system control interrupt on Intel chipsets.
global irq9_handler
irq9_handler:
	cli
	push byte 9
	push byte 41
	jmp prep_irq_handler

; 10: This interrupt is left open for the use of peripherals.
global irq10_handler
irq10_handler:
	cli
	push byte 10
	push byte 42
	jmp prep_irq_handler

; 11: This interrupt is left open for the use of peripherals.
global irq11_handler
irq11_handler:
	cli
	push byte 11
	push byte 43
	jmp prep_irq_handler

; 12: Mouse on PS/2 connector.
global irq12_handler
irq12_handler:
	cli
	push byte 12
	push byte 44
	jmp prep_irq_handler

; 13: CPU co-processor or integrated floating point unit or inter-processor interrupt.
global irq13_handler
irq13_handler:
	cli
	push byte 13
	push byte 45
	jmp prep_irq_handler

; 14: Primary ATA channel.
global irq14_handler
irq14_handler:
	cli
	push byte 14
	push byte 46
	jmp prep_irq_handler

; 15: Secondary ATA channel.
global irq15_handler
irq15_handler:
	cli
	push byte 15
	push byte 47
	jmp prep_irq_handler
