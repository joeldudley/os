[extern isr_handler]

isr_prep:              ; Prepares the CPU to handle an interrupt.
	pusha              ; Saves the CPU's current state.
	mov ax, ds         ; Lower 16-bits of eax = ds.
	push eax           ; Save the data segment descriptor.
	mov ax, 0x10       ; Kernel data segment descriptor.
	mov ds, ax         ;   "
	mov es, ax         ;   "
	mov fs, ax         ;   "
	mov gs, ax         ;   "
	
	call isr_handler   ; Calls the C interrupt handler.
	
	pop eax            ; Restores the CPU's state.
	mov ds, ax         ;   "
	mov es, ax         ;   "
	mov fs, ax         ;   "
	mov gs, ax         ;   "
	popa               ;   "
	add esp, 8         ; Cleans up the pushed error code and pushed ISR number.
	sti                ;   "
	iret               ; Pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP.
	
; We don't get information about which interrupt was called when the handler is run, so we need to  
; create a different handler for each interrupt.
; Some interrupts push an error code onto the stack while others don't. We will push a dummy error 
; code onto the stack for the latter to ensure that all interrupts have the same stack structure.

; 0: Divide By Zero Exception
global isr0
isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_prep

; 1: Debug Exception
global isr1
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_prep

; 2: Non Maskable Interrupt Exception
global isr2
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_prep

; 3: Int 3 Exception
global isr3
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_prep

; 4: INTO Exception
global isr4
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_prep

; 5: Out of Bounds Exception
global isr5
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_prep

; 6: Invalid Opcode Exception
global isr6
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_prep

; 7: Coprocessor Not Available Exception
global isr7
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_prep

; 8: Double Fault Exception
global isr8
isr8:
    cli
    push byte 8        ; Pushes an error code onto the stack.
    jmp isr_prep

; 9: Coprocessor Segment Overrun Exception
global isr9
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_prep

; 10: Bad TSS Exception
global isr10
isr10:
    cli
    push byte 10       ; Pushes an error code onto the stack.
    jmp isr_prep

; 11: Segment Not Present Exception
global isr11
isr11:
    cli
    push byte 11       ; Pushes an error code onto the stack.
    jmp isr_prep

; 12: Stack Fault Exception
global isr12
isr12:
    cli
    push byte 12       ; Pushes an error code onto the stack.
    jmp isr_prep

; 13: General Protection Fault Exception
global isr13
isr13:
    cli
    push byte 13       ; Pushes an error code onto the stack.
    jmp isr_prep

; 14: Page Fault Exception
global isr14
isr14:
    cli
    push byte 14       ; Pushes an error code onto the stack.
    jmp isr_prep

; 15: Reserved Exception
global isr15
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_prep

; 16: Floating Point Exception
global isr16
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_prep

; 17: Alignment Check Exception
global isr17
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_prep

; 18: Machine Check Exception
global isr18
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_prep

; 19: Reserved
global isr19
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_prep

; 20: Reserved
global isr20
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_prep

; 21: Reserved
global isr21
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_prep

; 22: Reserved
global isr22
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_prep

; 23: Reserved
global isr23
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_prep

; 24: Reserved
global isr24
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_prep

; 25: Reserved
global isr25
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_prep

; 26: Reserved
global isr26
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_prep

; 27: Reserved
global isr27
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_prep

; 28: Reserved
global isr28
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_prep

; 29: Reserved
global isr29
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_prep

; 30: Reserved
global isr30
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_prep

; 31: Reserved
global isr31
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_prep