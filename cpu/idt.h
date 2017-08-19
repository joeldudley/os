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
} __attribute__((packed)) idt_handler_t ;  // `packed` prevents the compiler from padding the struct.

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

idt_handler_t idt_handlers[NUM_IDT_ENTRIES];    // Our array of interrupt handlers.
idt_t idt;                 						// Our Interrupt Table Descriptor.

void handle_interrupt(interrupt_registers_t r);
void add_interrupt_handler(int n, u32 handler);
void load_idt();
void build_and_load_idt();

/* Interrupt service routines reserved for CPU exceptions. */
extern void interrupt0();
extern void interrupt1();
extern void interrupt2();
extern void interrupt3();
extern void interrupt4();
extern void interrupt5();
extern void interrupt6();
extern void interrupt7();
extern void interrupt8();
extern void interrupt9();
extern void interrupt10();
extern void interrupt11();
extern void interrupt12();
extern void interrupt13();
extern void interrupt14();
extern void interrupt15();
extern void interrupt16();
extern void interrupt17();
extern void interrupt18();
extern void interrupt19();
extern void interrupt20();
extern void interrupt21();
extern void interrupt22();
extern void interrupt23();
extern void interrupt24();
extern void interrupt25();
extern void interrupt26();
extern void interrupt27();
extern void interrupt28();
extern void interrupt29();
extern void interrupt30();
extern void interrupt31();

#endif
