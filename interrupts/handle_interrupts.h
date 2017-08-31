#ifndef HANDLE_INTERRUPTS_H
#define HANDLE_INTERRUPTS_H

#include <stdint.h>

// The index at which we want to register each software handler for the hardware interrupts.
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

// Typedefs.

// Registers pushed before calling the interrupt handler.
typedef struct {
    uint32_t ds;                                      // Data segment selector.
    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax;  // Pushed by `pusha`.
    uint32_t interrupt_no, err_code;                  // Interrupt number and error code (if applicable).
    uint32_t eip, cs, eflags, esp, ss;            // Automatically pushed by the processor.
} interrupt_args_t;

typedef void (*isr_t)(interrupt_args_t*);  // A pointer to a handler function.

// Public functions.
void initialise_irqs();

void add_interrupt_handling_function_to_array(uint8_t idx, isr_t handler);

#endif
