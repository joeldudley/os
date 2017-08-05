#include "idt.h"
#include "../kernel/util.h"

/**
 * Defines a kernel-privilege, used 32-bit interrupt gate in the IDT.
 */
void set_idt_gate(int n, u32 handler) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    // A kernel-privilege, used 32-bit interrupt gate.
    idt[n].flags = 0x8E; 
    idt[n].high_offset = high_16(handler);
}

/**
 * Loads the IDT.
 */
void load_idt() {
    idt_reg.base = (u32) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    // `lidtl` is an assembly instruction to load the IDT.
    asm __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
