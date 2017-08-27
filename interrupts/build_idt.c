#include "build_idt.h"
#include "../utils/ports.h"

// The Interrupt Table Descriptor (IDT) allows us to define the handler routine to be called for
// each interrupt.

// Constants.
#define KERNEL_CS 0x08
#define NUM_IDT_ENTRIES 256

// Variables.

// The IDT.
idt_t idt;
// The array of handlers used to load the IDT.
asm_handler_t asm_handlers[NUM_IDT_ENTRIES];

// Private function declarations.
void add_handler_to_asm_handler_array(int idx, u32 handler);
void load_idt_from_asm_handler_array();

// Public functions.

/**
 * Builds an array of .asm interrupt handlers and uses it to load the IDT.
 * The IDT tells the processor which .asm handler to call for each interrupt.
 */
void build_and_load_idt() {
    // Add the special CPU-dedicated interrupt handlers to the interrupt array.
    add_handler_to_asm_handler_array(0, (u32) isr0_handler);
    add_handler_to_asm_handler_array(1, (u32) isr1_handler);
    add_handler_to_asm_handler_array(2, (u32) isr2_handler);
    add_handler_to_asm_handler_array(3, (u32) isr3_handler);
    add_handler_to_asm_handler_array(4, (u32) isr4_handler);
    add_handler_to_asm_handler_array(5, (u32) isr5_handler);
    add_handler_to_asm_handler_array(6, (u32) isr6_handler);
    add_handler_to_asm_handler_array(7, (u32) isr7_handler);
    add_handler_to_asm_handler_array(8, (u32) isr8_handler);
    add_handler_to_asm_handler_array(9, (u32) isr9_handler);
    add_handler_to_asm_handler_array(10, (u32) isr10_handler);
    add_handler_to_asm_handler_array(11, (u32) isr11_handler);
    add_handler_to_asm_handler_array(12, (u32) isr12_handler);
    add_handler_to_asm_handler_array(13, (u32) isr13_handler);
    add_handler_to_asm_handler_array(14, (u32) isr14_handler);
    add_handler_to_asm_handler_array(15, (u32) isr15_handler);
    add_handler_to_asm_handler_array(16, (u32) isr16_handler);
    add_handler_to_asm_handler_array(17, (u32) isr17_handler);
    add_handler_to_asm_handler_array(18, (u32) isr18_handler);
    add_handler_to_asm_handler_array(19, (u32) isr19_handler);
    add_handler_to_asm_handler_array(20, (u32) isr20_handler);
    add_handler_to_asm_handler_array(21, (u32) isr21_handler);
    add_handler_to_asm_handler_array(22, (u32) isr22_handler);
    add_handler_to_asm_handler_array(23, (u32) isr23_handler);
    add_handler_to_asm_handler_array(24, (u32) isr24_handler);
    add_handler_to_asm_handler_array(25, (u32) isr25_handler);
    add_handler_to_asm_handler_array(26, (u32) isr26_handler);
    add_handler_to_asm_handler_array(27, (u32) isr27_handler);
    add_handler_to_asm_handler_array(28, (u32) isr28_handler);
    add_handler_to_asm_handler_array(29, (u32) isr29_handler);
    add_handler_to_asm_handler_array(30, (u32) isr30_handler);
    add_handler_to_asm_handler_array(31, (u32) isr31_handler);

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

    // Add the hardware interrupt handlers to the interrupt array.
    add_handler_to_asm_handler_array(32, (u32) irq0_handler);
    add_handler_to_asm_handler_array(33, (u32) irq1_handler);
    add_handler_to_asm_handler_array(34, (u32) irq2_handler);
    add_handler_to_asm_handler_array(35, (u32) irq3_handler);
    add_handler_to_asm_handler_array(36, (u32) irq4_handler);
    add_handler_to_asm_handler_array(37, (u32) irq5_handler);
    add_handler_to_asm_handler_array(38, (u32) irq6_handler);
    add_handler_to_asm_handler_array(39, (u32) irq7_handler);
    add_handler_to_asm_handler_array(40, (u32) irq8_handler);
    add_handler_to_asm_handler_array(41, (u32) irq9_handler);
    add_handler_to_asm_handler_array(42, (u32) irq10_handler);
    add_handler_to_asm_handler_array(43, (u32) irq11_handler);
    add_handler_to_asm_handler_array(44, (u32) irq12_handler);
    add_handler_to_asm_handler_array(45, (u32) irq13_handler);
    add_handler_to_asm_handler_array(46, (u32) irq14_handler);
    add_handler_to_asm_handler_array(47, (u32) irq15_handler);

    load_idt_from_asm_handler_array();
}

// Private functions.

/**
 * Adds a kernel-privilege, used 32-bit interrupt handler to the array of .asm interrupt handlers.
 *
 * idx: The index of the interrupt for which we are adding a handler.
 * handler: The interrupt handler definition.
 */
void add_handler_to_asm_handler_array(int idx, u32 handler) {
    asm_handlers[idx].low_offset = low_16(handler);
    asm_handlers[idx].sel = KERNEL_CS;
    asm_handlers[idx].always0 = 0;
    // Flags indicate a kernel-privilege, used 32-bit interrupt gate.
    asm_handlers[idx].flags = 0x8E;
    asm_handlers[idx].high_offset = high_16(handler);
}

/**
 * Loads the IDT using the .asm interrupt handler array.
 */
void load_idt_from_asm_handler_array() {
    idt.base = (u32) &asm_handlers;
    idt.limit = NUM_IDT_ENTRIES * sizeof(asm_handler_t) - 1;
    // `lidtl` is an assembly instruction to load the IDT.
    asm volatile("lidtl (%0)" : : "r" (&idt));
}