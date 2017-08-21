[extern handle_isr]
[extern handle_irq]

prep_isr:    ; Prepares the CPU to handle an interrupt.
	pusha                  ; Saves the CPU's current state.
	mov ax, ds             ; Lower 16-bits of eax = ds.
	push eax               ; Save the data segment descriptor.
	mov ax, 0x10           ; Kernel data segment descriptor.
	mov ds, ax             ;   "
	mov es, ax             ;   "
	mov fs, ax             ;   "
	mov gs, ax             ;   "
	
	call handle_isr        ; Calls the C interrupt handler.
	
	pop eax                ; Restores the CPU's state.
	mov ds, ax             ;   "
	mov es, ax             ;   "
	mov fs, ax             ;   "
	mov gs, ax             ;   "
	popa                   ;   "
	add esp, 8             ; Cleans up the pushed error code and pushed interrupt number.
	sti                    ;   "
	iret                   ; Pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP.

; As above, except for two lines.
prep_irq_handler:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call handle_irq        ; Calls the C interrupt request.

    pop ebx  			   ; Different than the ISR code. TODO: Find out why.
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret

; INTERRUPT HANDLERS

; We don't get information about which interrupt was called when the handler is run, so we need to
; create a different handler for each interrupt.

; Special CPU-dedicated interrupt handlers.

; Some of the CPU-dedicated interrupts push an error code onto the stack while others don't. We
; will push a dummy error code onto the stack in the latter case to ensure that all interrupts
; have the same stack structure.

; 0: Divide by zero exception.
global isr0
isr0:
    cli
    push byte 0
    push byte 0
    jmp prep_isr

; 1: Debug exception.
global isr1
isr1:
    cli
    push byte 0
    push byte 1
    jmp prep_isr

; 2: Non maskable interrupt exception.
global isr2
isr2:
    cli
    push byte 0
    push byte 2
    jmp prep_isr

; 3: Int 3 exception
global isr3
isr3:
    cli
    push byte 0
    push byte 3
    jmp prep_isr

; 4: INTO exception.
global isr4
isr4:
    cli
    push byte 0
    push byte 4
    jmp prep_isr

; 5: Out of bounds exception.
global isr5
isr5:
    cli
    push byte 0
    push byte 5
    jmp prep_isr

; 6: Invalid opcode exception.
global isr6
isr6:
    cli
    push byte 0
    push byte 6
    jmp prep_isr

; 7: Coprocessor not available exception.
global isr7
isr7:
    cli
    push byte 0
    push byte 7
    jmp prep_isr

; 8: Double fault exception.
global isr8
isr8:
    cli
    ; Pushes an error code onto the stack.
    push byte 8
    jmp prep_isr

; 9: Coprocessor segment overrun exception.
global isr9
isr9:
    cli
    push byte 0
    push byte 9
    jmp prep_isr

; 10: Bad TSS exception.
global isr10
isr10:
    cli
    ; Pushes an error code onto the stack.
    push byte 10
    jmp prep_isr

; 11: Segment not present exception.
global isr11
isr11:
    cli
    ; Pushes an error code onto the stack.
    push byte 11
    jmp prep_isr

; 12: Stack fault exception.
global isr12
isr12:
    cli
    ; Pushes an error code onto the stack.
    push byte 12
    jmp prep_isr

; 13: General protection fault exception.
global isr13
isr13:
    cli
    ; Pushes an error code onto the stack.
    push byte 13
    jmp prep_isr

; 14: Page fault exception.
global isr14
isr14:
    cli
    ; Pushes an error code onto the stack.
    push byte 14
    jmp prep_isr

; 15: Reserved exception.
global isr15
isr15:
    cli
    push byte 0
    push byte 15
    jmp prep_isr

; 16: Floating point exception.
global isr16
isr16:
    cli
    push byte 0
    push byte 16
    jmp prep_isr

; 17: Alignment check exception.
global isr17
isr17:
    cli
    push byte 0
    push byte 17
    jmp prep_isr

; 18: Machine check exception.
global isr18
isr18:
    cli
    push byte 0
    push byte 18
    jmp prep_isr

; 19: Reserved.
global isr19
isr19:
    cli
    push byte 0
    push byte 19
    jmp prep_isr

; 20: Reserved.
global isr20
isr20:
    cli
    push byte 0
    push byte 20
    jmp prep_isr

; 21: Reserved.
global isr21
isr21:
    cli
    push byte 0
    push byte 21
    jmp prep_isr

; 22: Reserved.
global isr22
isr22:
    cli
    push byte 0
    push byte 22
    jmp prep_isr

; 23: Reserved.
global isr23
isr23:
    cli
    push byte 0
    push byte 23
    jmp prep_isr

; 24: Reserved.
global isr24
isr24:
    cli
    push byte 0
    push byte 24
    jmp prep_isr

; 25: Reserved.
global isr25
isr25:
    cli
    push byte 0
    push byte 25
    jmp prep_isr

; 26: Reserved.
global isr26
isr26:
    cli
    push byte 0
    push byte 26
    jmp prep_isr

; 27: Reserved.
global isr27
isr27:
    cli
    push byte 0
    push byte 27
    jmp prep_isr

; 28: Reserved.
global isr28
isr28:
    cli
    push byte 0
    push byte 28
    jmp prep_isr

; 29: Reserved.
global isr29
isr29:
    cli
    push byte 0
    push byte 29
    jmp prep_isr

; 30: Reserved.
global isr30
isr30:
    cli
    push byte 0
    push byte 30
    jmp prep_isr

; 31: Reserved.
global isr31
isr31:
    cli
    push byte 0
    push byte 31
    jmp prep_isr

; Hardware interrupt handlers.

; 0: System timer.
global irq0
irq0:
	cli
	push byte 0
	push byte 32
	jmp prep_irq_handler

; 1: Keyboard controller.
global irq1
irq1:
	cli
	push byte 1
	push byte 33
	jmp prep_irq_handler

; 2: Cascaded signals from IRQs 8-15.
global irq2
irq2:
	cli
	push byte 2
	push byte 34
	jmp prep_irq_handler

; 3: Serial port controller for serial port 2.
global irq3
irq3:
	cli
	push byte 3
	push byte 35
	jmp prep_irq_handler

; 4: Serial port controller for serial port 1.
global irq4
irq4:
	cli
	push byte 4
	push byte 36
	jmp prep_irq_handler

; 5: Parallel port 2 and 3 or sound card.
global irq5
irq5:
	cli
	push byte 5
	push byte 37
	jmp prep_irq_handler

; 6: Floppy disk controller.
global irq6
irq6:
	cli
	push byte 6
	push byte 38
	jmp prep_irq_handler

; 7: Parallel port 1.
global irq7
irq7:
	cli
	push byte 7
	push byte 39
	jmp prep_irq_handler

; 8: Real-time clock.
global irq8
irq8:
	cli
	push byte 8
	push byte 40
	jmp prep_irq_handler

; 9: Advanced Configuration and Power Interface system control interrupt on Intel chipsets.
global irq9
irq9:
	cli
	push byte 9
	push byte 41
	jmp prep_irq_handler

; 10: This interrupt is left open for the use of peripherals.
global irq10
irq10:
	cli
	push byte 10
	push byte 42
	jmp prep_irq_handler

; 11: This interrupt is left open for the use of peripherals.
global irq11
irq11:
	cli
	push byte 11
	push byte 43
	jmp prep_irq_handler

; 12: Mouse on PS/2 connector.
global irq12
irq12:
	cli
	push byte 12
	push byte 44
	jmp prep_irq_handler

; 13: CPU co-processor or integrated floating point unit or inter-processor interrupt.
global irq13
irq13:
	cli
	push byte 13
	push byte 45
	jmp prep_irq_handler

; 14: Primary ATA channel.
global irq14
irq14:
	cli
	push byte 14
	push byte 46
	jmp prep_irq_handler

; 15: Secondary ATA channel.
global irq15
irq15:
	cli
	push byte 15
	push byte 47
	jmp prep_irq_handler
