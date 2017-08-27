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
extern void isr0_handler_asm();
extern void isr1_handler_asm();
extern void isr2_handler_asm();
extern void isr3_handler_asm();
extern void isr4_handler_asm();
extern void isr5_handler_asm();
extern void isr6_handler_asm();
extern void isr7_handler_asm();
extern void isr8_handler_asm();
extern void isr9_handler_asm();
extern void isr10_handler_asm();
extern void isr11_handler_asm();
extern void isr12_handler_asm();
extern void isr13_handler_asm();
extern void isr14_handler_asm();
extern void isr15_handler_asm();
extern void isr16_handler_asm();
extern void isr17_handler_asm();
extern void isr18_handler_asm();
extern void isr19_handler_asm();
extern void isr20_handler_asm();
extern void isr21_handler_asm();
extern void isr22_handler_asm();
extern void isr23_handler_asm();
extern void isr24_handler_asm();
extern void isr25_handler_asm();
extern void isr26_handler_asm();
extern void isr27_handler_asm();
extern void isr28_handler_asm();
extern void isr29_handler_asm();
extern void isr30_handler_asm();
extern void isr31_handler_asm();

// Interrupt request definitions.
extern void irq0_handler_asm();
extern void irq1_handler_asm();
extern void irq2_handler_asm();
extern void irq3_handler_asm();
extern void irq4_handler_asm();
extern void irq5_handler_asm();
extern void irq6_handler_asm();
extern void irq7_handler_asm();
extern void irq8_handler_asm();
extern void irq9_handler_asm();
extern void irq10_handler_asm();
extern void irq11_handler_asm();
extern void irq12_handler_asm();
extern void irq13_handler_asm();
extern void irq14_handler_asm();
extern void irq15_handler_asm();

#endif
