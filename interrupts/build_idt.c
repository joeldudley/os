#include "build_idt.h"
#include "../utils/ports.h"
#include "../utils/memory.h"

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
void add_handler_to_asm_handler_array(int idx, uint32_t handler);
void load_idt_from_asm_handler_array();

// Public functions.

/**
 * Builds an array of .asm interrupt handlers and uses it to load the IDT.
 * The IDT tells the processor which .asm handler to call for each interrupt.
 */
void build_and_load_idt() {
    // Add the special CPU-dedicated interrupt handlers to the interrupt array.
    add_handler_to_asm_handler_array(0, (uint32_t) isr0_handler);
    add_handler_to_asm_handler_array(1, (uint32_t) isr1_handler);
    add_handler_to_asm_handler_array(2, (uint32_t) isr2_handler);
    add_handler_to_asm_handler_array(3, (uint32_t) isr3_handler);
    add_handler_to_asm_handler_array(4, (uint32_t) isr4_handler);
    add_handler_to_asm_handler_array(5, (uint32_t) isr5_handler);
    add_handler_to_asm_handler_array(6, (uint32_t) isr6_handler);
    add_handler_to_asm_handler_array(7, (uint32_t) isr7_handler);
    add_handler_to_asm_handler_array(8, (uint32_t) isr8_handler);
    add_handler_to_asm_handler_array(9, (uint32_t) isr9_handler);
    add_handler_to_asm_handler_array(10, (uint32_t) isr10_handler);
    add_handler_to_asm_handler_array(11, (uint32_t) isr11_handler);
    add_handler_to_asm_handler_array(12, (uint32_t) isr12_handler);
    add_handler_to_asm_handler_array(13, (uint32_t) isr13_handler);
    add_handler_to_asm_handler_array(14, (uint32_t) isr14_handler);
    add_handler_to_asm_handler_array(15, (uint32_t) isr15_handler);
    add_handler_to_asm_handler_array(16, (uint32_t) isr16_handler);
    add_handler_to_asm_handler_array(17, (uint32_t) isr17_handler);
    add_handler_to_asm_handler_array(18, (uint32_t) isr18_handler);
    add_handler_to_asm_handler_array(19, (uint32_t) isr19_handler);
    add_handler_to_asm_handler_array(20, (uint32_t) isr20_handler);
    add_handler_to_asm_handler_array(21, (uint32_t) isr21_handler);
    add_handler_to_asm_handler_array(22, (uint32_t) isr22_handler);
    add_handler_to_asm_handler_array(23, (uint32_t) isr23_handler);
    add_handler_to_asm_handler_array(24, (uint32_t) isr24_handler);
    add_handler_to_asm_handler_array(25, (uint32_t) isr25_handler);
    add_handler_to_asm_handler_array(26, (uint32_t) isr26_handler);
    add_handler_to_asm_handler_array(27, (uint32_t) isr27_handler);
    add_handler_to_asm_handler_array(28, (uint32_t) isr28_handler);
    add_handler_to_asm_handler_array(29, (uint32_t) isr29_handler);
    add_handler_to_asm_handler_array(30, (uint32_t) isr30_handler);
    add_handler_to_asm_handler_array(31, (uint32_t) isr31_handler);

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
    add_handler_to_asm_handler_array(32, (uint32_t) irq0_handler);
    add_handler_to_asm_handler_array(33, (uint32_t) irq1_handler);
    add_handler_to_asm_handler_array(34, (uint32_t) irq2_handler);
    add_handler_to_asm_handler_array(35, (uint32_t) irq3_handler);
    add_handler_to_asm_handler_array(36, (uint32_t) irq4_handler);
    add_handler_to_asm_handler_array(37, (uint32_t) irq5_handler);
    add_handler_to_asm_handler_array(38, (uint32_t) irq6_handler);
    add_handler_to_asm_handler_array(39, (uint32_t) irq7_handler);
    add_handler_to_asm_handler_array(40, (uint32_t) irq8_handler);
    add_handler_to_asm_handler_array(41, (uint32_t) irq9_handler);
    add_handler_to_asm_handler_array(42, (uint32_t) irq10_handler);
    add_handler_to_asm_handler_array(43, (uint32_t) irq11_handler);
    add_handler_to_asm_handler_array(44, (uint32_t) irq12_handler);
    add_handler_to_asm_handler_array(45, (uint32_t) irq13_handler);
    add_handler_to_asm_handler_array(46, (uint32_t) irq14_handler);
    add_handler_to_asm_handler_array(47, (uint32_t) irq15_handler);

    load_idt_from_asm_handler_array();
}

// Private functions.

/**
 * Adds a kernel-privilege, used 32-bit interrupt handler to the array of .asm interrupt handlers.
 *
 * idx: The index of the interrupt for which we are adding a handler.
 * handler: The interrupt handler definition.
 */
void add_handler_to_asm_handler_array(int idx, uint32_t handler) {
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
    idt.base = (uint32_t) &asm_handlers;
    idt.limit = NUM_IDT_ENTRIES * sizeof(asm_handler_t) - 1;
    // `lidtl` is an assembly instruction to load the IDT.
    asm volatile("lidtl (%0)" : : "r" (&idt));
}