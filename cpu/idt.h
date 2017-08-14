#ifndef IDT_H
#define IDT_H

#include "types.h"

// Constants.
#define KERNEL_CS 0x08  
#define NUM_IDT_ENTRIES 256

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
} __attribute__((packed)) idt_gate_t ;  // `packed` prevents the compiler from padding the struct.

// An Interrupt Table Descriptor.
typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idt_register_t;

idt_gate_t idt[NUM_IDT_ENTRIES];        // Our array of interrupt handlers.
idt_register_t idt_reg;                 // Our Interrupt Table Descriptor.

void set_idt_gate(int n, u32 handler);
void load_idt();

#endif
