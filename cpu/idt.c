#include "idt.h"
#include "../kernel/screen.h"
#include "../kernel/util.h"

/**
 * Installs the handlers for CPU exception interrupts in the IDT and loads the IDT.
 */
void build_and_load_idt() {
    add_interrupt_handler(0, (u32) interrupt0);
    add_interrupt_handler(1, (u32) interrupt1);
    add_interrupt_handler(2, (u32) interrupt2);
    add_interrupt_handler(3, (u32) interrupt3);
    add_interrupt_handler(4, (u32) interrupt4);
    add_interrupt_handler(5, (u32) interrupt5);
    add_interrupt_handler(6, (u32) interrupt6);
    add_interrupt_handler(7, (u32) interrupt7);
    add_interrupt_handler(8, (u32) interrupt8);
    add_interrupt_handler(9, (u32) interrupt9);
    add_interrupt_handler(10, (u32) interrupt10);
    add_interrupt_handler(11, (u32) interrupt11);
    add_interrupt_handler(12, (u32) interrupt12);
    add_interrupt_handler(13, (u32) interrupt13);
    add_interrupt_handler(14, (u32) interrupt14);
    add_interrupt_handler(15, (u32) interrupt15);
    add_interrupt_handler(16, (u32) interrupt16);
    add_interrupt_handler(17, (u32) interrupt17);
    add_interrupt_handler(18, (u32) interrupt18);
    add_interrupt_handler(19, (u32) interrupt19);
    add_interrupt_handler(20, (u32) interrupt20);
    add_interrupt_handler(21, (u32) interrupt21);
    add_interrupt_handler(22, (u32) interrupt22);
    add_interrupt_handler(23, (u32) interrupt23);
    add_interrupt_handler(24, (u32) interrupt24);
    add_interrupt_handler(25, (u32) interrupt25);
    add_interrupt_handler(26, (u32) interrupt26);
    add_interrupt_handler(27, (u32) interrupt27);
    add_interrupt_handler(28, (u32) interrupt28);
    add_interrupt_handler(29, (u32) interrupt29);
    add_interrupt_handler(30, (u32) interrupt30);
    add_interrupt_handler(31, (u32) interrupt31);

    load_idt();
}

/**
 * Adds a kernel-privilege, used 32-bit interrupt gate to the IDT.
 */
void add_interrupt_handler(int n, u32 handler) {
    idt_handlers[n].low_offset = low_16(handler);
    idt_handlers[n].sel = KERNEL_CS;
    idt_handlers[n].always0 = 0;
    // A kernel-privilege, used 32-bit interrupt gate.
    idt_handlers[n].flags = 0x8E;
    idt_handlers[n].high_offset = high_16(handler);
}

/**
 * Loads the IDT.
 */
void load_idt() {
    idt.base = (u32) &idt_handlers;
    idt.limit = NUM_IDT_ENTRIES * sizeof(idt_handler_t) - 1;
    // `lidtl` is an assembly instruction to load the IDT.
    asm volatile("lidtl (%0)" : : "r" (&idt));
}

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
 * Handles an interrupt.
 */
void handle_interrupt(interrupt_registers_t r) {
    print("Received an interrupt: ");
    char interrupt_number[3];
    int_to_ascii(r.interrupt_no, interrupt_number);
    print(interrupt_number);
    print("\n");
    print(exception_messages[r.interrupt_no]);
    print("\n");
}
