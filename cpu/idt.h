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
} __attribute__((packed)) interrupt_t ;  // `packed` prevents the compiler from padding the struct.

// An Interrupt Table Descriptor.
typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idt_t;

// Registers pushed before calling the interrupt handler.
typedef struct {
   u32 ds;                                      // Data segment selector.
   u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;  // Pushed by `pusha`.
   u32 interrupt_no, err_code;                  // Interrupt number and error code (if applicable).
   u32 eip, cs, eflags, useresp, ss;            // Automatically pushed by the processor.
} interrupt_registers_t;

idt_t idt;                 						// The Interrupt Table Descriptor.
interrupt_t interrupts[NUM_IDT_ENTRIES];        // The IDT's array of interrupts.
typedef void (*isr_t) (interrupt_registers_t);  // A pointer to a handler function.
isr_t interrupt_handlers[NUM_IDT_ENTRIES];		// An array of pointers to interrupt handlers.

void build_and_load_idt();
void add_interrupt_gate(int n, u32 handler);
void load_idt();
void handle_isr(interrupt_registers_t r);
void register_interrupt_handler(u8 n, isr_t handler);
void handle_irq(interrupt_registers_t r);

/* Interrupt handler definitions. */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/* Interrupt request definitions. */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// If these are lower case, they'll clash with the function names.
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

#endif
