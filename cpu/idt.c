#include "idt.h"
#include "../kernel/screen.h"
#include "../kernel/util.h"
#include "../kernel/ports.h"

/**
 * Stores the message associated with each interrupt.
 */
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/**
 * Adds the interrupt handlers and interrupt requests to the IDT and loads the IDT.
 */
void build_and_load_idt() {
	// Add the interrupt handlers.
    add_interrupt_gate(0, (u32) isr0);
    add_interrupt_gate(1, (u32) isr1);
    add_interrupt_gate(2, (u32) isr2);
    add_interrupt_gate(3, (u32) isr3);
    add_interrupt_gate(4, (u32) isr4);
    add_interrupt_gate(5, (u32) isr5);
    add_interrupt_gate(6, (u32) isr6);
    add_interrupt_gate(7, (u32) isr7);
    add_interrupt_gate(8, (u32) isr8);
    add_interrupt_gate(9, (u32) isr9);
    add_interrupt_gate(10, (u32) isr10);
    add_interrupt_gate(11, (u32) isr11);
    add_interrupt_gate(12, (u32) isr12);
    add_interrupt_gate(13, (u32) isr13);
    add_interrupt_gate(14, (u32) isr14);
    add_interrupt_gate(15, (u32) isr15);
    add_interrupt_gate(16, (u32) isr16);
    add_interrupt_gate(17, (u32) isr17);
    add_interrupt_gate(18, (u32) isr18);
    add_interrupt_gate(19, (u32) isr19);
    add_interrupt_gate(20, (u32) isr20);
    add_interrupt_gate(21, (u32) isr21);
    add_interrupt_gate(22, (u32) isr22);
    add_interrupt_gate(23, (u32) isr23);
    add_interrupt_gate(24, (u32) isr24);
    add_interrupt_gate(25, (u32) isr25);
    add_interrupt_gate(26, (u32) isr26);
    add_interrupt_gate(27, (u32) isr27);
    add_interrupt_gate(28, (u32) isr28);
    add_interrupt_gate(29, (u32) isr29);
    add_interrupt_gate(30, (u32) isr30);
    add_interrupt_gate(31, (u32) isr31);

    // Remap the PIC.
    // TODO: Find out what this means.
   port_write_byte(0x20, 0x11);
   port_write_byte(0xA0, 0x11);
   port_write_byte(0x21, 0x20);
   port_write_byte(0xA1, 0x28);
   port_write_byte(0x21, 0x04);
   port_write_byte(0xA1, 0x02);
   port_write_byte(0x21, 0x01);
   port_write_byte(0xA1, 0x01);
   port_write_byte(0x21, 0x0);
   port_write_byte(0xA1, 0x0);

    // Add the interrupt requests.
    add_interrupt_gate(32, (u32) irq0);
    add_interrupt_gate(33, (u32) irq1);
    add_interrupt_gate(34, (u32) irq2);
    add_interrupt_gate(35, (u32) irq3);
    add_interrupt_gate(36, (u32) irq4);
    add_interrupt_gate(37, (u32) irq5);
    add_interrupt_gate(38, (u32) irq6);
    add_interrupt_gate(39, (u32) irq7);
    add_interrupt_gate(40, (u32) irq8);
    add_interrupt_gate(41, (u32) irq9);
    add_interrupt_gate(42, (u32) irq10);
    add_interrupt_gate(43, (u32) irq11);
    add_interrupt_gate(44, (u32) irq12);
    add_interrupt_gate(45, (u32) irq13);
    add_interrupt_gate(46, (u32) irq14);
    add_interrupt_gate(47, (u32) irq15);

    load_idt();
}

/**
 * Adds a kernel-privilege, used 32-bit interrupt gate to the IDT.
 */
void add_interrupt_gate(int n, u32 handler) {
	interrupt_gates[n].low_offset = low_16(handler);
	interrupt_gates[n].sel = KERNEL_CS;
	interrupt_gates[n].always0 = 0;
    // A kernel-privilege, used 32-bit interrupt gate.
	interrupt_gates[n].flags = 0x8E;
	interrupt_gates[n].high_offset = high_16(handler);
}

/**
 * Loads the IDT.
 */
void load_idt() {
    idt.base = (u32) &interrupt_gates;
    idt.limit = NUM_IDT_ENTRIES * sizeof(interrupt_handler_t) - 1;
    // `lidtl` is an assembly instruction to load the IDT.
    asm volatile("lidtl (%0)" : : "r" (&idt));
}

/**
 * Handles an interrupt.
 */
void handle_isr(interrupt_registers_t r) {
    print("Received an interrupt: ");
    char interrupt_number[3];
    int_to_ascii(r.interrupt_no, interrupt_number);
    print(interrupt_number);
    print("\n");
    print(exception_messages[r.interrupt_no]);
    print("\n");
}

// NEW STUFF
void register_interrupt_handler(u8 n, isr_t *handler) {
    interrupt_handlers[n] = handler;
}

void handle_irq(interrupt_registers_t r) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */
    if (r.interrupt_no >= 40) port_write_byte(0xA0, 0x20); /* slave */
    port_write_byte(0x20, 0x20); /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r.interrupt_no] != 0) {
    	isr_t handler = interrupt_handlers[r.interrupt_no];
        handler(r);
    }
}
