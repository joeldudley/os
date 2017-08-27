#ifndef BUILD_IDT_H
#define BUILD_IDT_H

#include "../utils/types.h"

// Typedefs.

// An interrupt handler.
typedef struct {
    u16 low_offset;                     // Lower 16 bits of the interrupt service routine's address.
    u16 sel;                            // The segment selector.
    u8 always0;                         // Always set to 0.
    /* * Bits 0-3: The gate's type:
         * 0101 = 32-bit task gate
         * 0110 = 16-bit interrupt gate
         * 0111 = 16-bit trap gate
         * 1110 = 32-bit interrupt gate
         * 1111 = 32-bit trap gate
       * Bit 4: Set to 0 for interrupt/trap gates
       * Bits 5-6: The required privilege level (from 0 = kernel to 3 = user)
       * Bit 7: Set to 0 for unused interrupts */
    u8 flags;
    u16 high_offset;                    // Higher 16 bits of the interrupt service routine's address.
} __attribute__((packed)) asm_handler_t ;  // `packed` prevents the compiler from padding the struct.

// An Interrupt Table Descriptor.
typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idt_t;

// Public functions.

void build_and_load_idt();

// Interrupt definitions.
extern void isr0_handler();
extern void isr1_handler();
extern void isr2_handler();
extern void isr3_handler();
extern void isr4_handler();
extern void isr5_handler();
extern void isr6_handler();
extern void isr7_handler();
extern void isr8_handler();
extern void isr9_handler();
extern void isr10_handler();
extern void isr11_handler();
extern void isr12_handler();
extern void isr13_handler();
extern void isr14_handler();
extern void isr15_handler();
extern void isr16_handler();
extern void isr17_handler();
extern void isr18_handler();
extern void isr19_handler();
extern void isr20_handler();
extern void isr21_handler();
extern void isr22_handler();
extern void isr23_handler();
extern void isr24_handler();
extern void isr25_handler();
extern void isr26_handler();
extern void isr27_handler();
extern void isr28_handler();
extern void isr29_handler();
extern void isr30_handler();
extern void isr31_handler();

// Interrupt request definitions.
extern void irq0_handler();
extern void irq1_handler();
extern void irq2_handler();
extern void irq3_handler();
extern void irq4_handler();
extern void irq5_handler();
extern void irq6_handler();
extern void irq7_handler();
extern void irq8_handler();
extern void irq9_handler();
extern void irq10_handler();
extern void irq11_handler();
extern void irq12_handler();
extern void irq13_handler();
extern void irq14_handler();
extern void irq15_handler();

#endif
