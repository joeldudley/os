[extern handle_interrupt]

prep_interrupt:            ; Prepares the CPU to handle an interrupt.
	pusha                  ; Saves the CPU's current state.
	mov ax, ds             ; Lower 16-bits of eax = ds.
	push eax               ; Save the data segment descriptor.
	mov ax, 0x10           ; Kernel data segment descriptor.
	mov ds, ax             ;   "
	mov es, ax             ;   "
	mov fs, ax             ;   "
	mov gs, ax             ;   "
	
	call handle_interrupt  ; Calls the C interrupt handler.
	
	pop eax                ; Restores the CPU's state.
	mov ds, ax             ;   "
	mov es, ax             ;   "
	mov fs, ax             ;   "
	mov gs, ax             ;   "
	popa                   ;   "
	add esp, 8             ; Cleans up the pushed error code and pushed interrupt number.
	sti                    ;   "
	iret                   ; Pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP.
	
; We don't get information about which interrupt was called when the handler is run, so we need to  
; create a different handler for each interrupt.
; Some interrupts push an error code onto the stack while others don't. We will push a dummy error 
; code onto the stack for the latter to ensure that all interrupts have the same stack structure.

; 0: Divide By Zero Exception
global interrupt0
interrupt0:
    cli
    push byte 0
    push byte 0
    jmp prep_interrupt

; 1: Debug Exception
global interrupt1
interrupt1:
    cli
    push byte 0
    push byte 1
    jmp prep_interrupt

; 2: Non Maskable Interrupt Exception
global interrupt2
interrupt2:
    cli
    push byte 0
    push byte 2
    jmp prep_interrupt

; 3: Int 3 Exception
global interrupt3
interrupt3:
    cli
    push byte 0
    push byte 3
    jmp prep_interrupt

; 4: INTO Exception
global interrupt4
interrupt4:
    cli
    push byte 0
    push byte 4
    jmp prep_interrupt

; 5: Out of Bounds Exception
global interrupt5
interrupt5:
    cli
    push byte 0
    push byte 5
    jmp prep_interrupt

; 6: Invalid Opcode Exception
global interrupt6
interrupt6:
    cli
    push byte 0
    push byte 6
    jmp prep_interrupt

; 7: Coprocessor Not Available Exception
global interrupt7
interrupt7:
    cli
    push byte 0
    push byte 7
    jmp prep_interrupt

; 8: Double Fault Exception
global interrupt8
interrupt8:
    cli
    push byte 8        ; Pushes an error code onto the stack.
    jmp prep_interrupt

; 9: Coprocessor Segment Overrun Exception
global interrupt9
interrupt9:
    cli
    push byte 0
    push byte 9
    jmp prep_interrupt

; 10: Bad TSS Exception
global interrupt10
interrupt10:
    cli
    push byte 10       ; Pushes an error code onto the stack.
    jmp prep_interrupt

; 11: Segment Not Present Exception
global interrupt11
interrupt11:
    cli
    push byte 11       ; Pushes an error code onto the stack.
    jmp prep_interrupt

; 12: Stack Fault Exception
global interrupt12
interrupt12:
    cli
    push byte 12       ; Pushes an error code onto the stack.
    jmp prep_interrupt

; 13: General Protection Fault Exception
global interrupt13
interrupt13:
    cli
    push byte 13       ; Pushes an error code onto the stack.
    jmp prep_interrupt

; 14: Page Fault Exception
global interrupt14
interrupt14:
    cli
    push byte 14       ; Pushes an error code onto the stack.
    jmp prep_interrupt

; 15: Reserved Exception
global interrupt15
interrupt15:
    cli
    push byte 0
    push byte 15
    jmp prep_interrupt

; 16: Floating Point Exception
global interrupt16
interrupt16:
    cli
    push byte 0
    push byte 16
    jmp prep_interrupt

; 17: Alignment Check Exception
global interrupt17
interrupt17:
    cli
    push byte 0
    push byte 17
    jmp prep_interrupt

; 18: Machine Check Exception
global interrupt18
interrupt18:
    cli
    push byte 0
    push byte 18
    jmp prep_interrupt

; 19: Reserved
global interrupt19
interrupt19:
    cli
    push byte 0
    push byte 19
    jmp prep_interrupt

; 20: Reserved
global interrupt20
interrupt20:
    cli
    push byte 0
    push byte 20
    jmp prep_interrupt

; 21: Reserved
global interrupt21
interrupt21:
    cli
    push byte 0
    push byte 21
    jmp prep_interrupt

; 22: Reserved
global interrupt22
interrupt22:
    cli
    push byte 0
    push byte 22
    jmp prep_interrupt

; 23: Reserved
global interrupt23
interrupt23:
    cli
    push byte 0
    push byte 23
    jmp prep_interrupt

; 24: Reserved
global interrupt24
interrupt24:
    cli
    push byte 0
    push byte 24
    jmp prep_interrupt

; 25: Reserved
global interrupt25
interrupt25:
    cli
    push byte 0
    push byte 25
    jmp prep_interrupt

; 26: Reserved
global interrupt26
interrupt26:
    cli
    push byte 0
    push byte 26
    jmp prep_interrupt

; 27: Reserved
global interrupt27
interrupt27:
    cli
    push byte 0
    push byte 27
    jmp prep_interrupt

; 28: Reserved
global interrupt28
interrupt28:
    cli
    push byte 0
    push byte 28
    jmp prep_interrupt

; 29: Reserved
global interrupt29
interrupt29:
    cli
    push byte 0
    push byte 29
    jmp prep_interrupt

; 30: Reserved
global interrupt30
interrupt30:
    cli
    push byte 0
    push byte 30
    jmp prep_interrupt

; 31: Reserved
global interrupt31
interrupt31:
    cli
    push byte 0
    push byte 31
    jmp prep_interrupt