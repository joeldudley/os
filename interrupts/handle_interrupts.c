#include "handle_interrupts.h"
#include "../utils/ports.h"
#include "../drivers/screen.h"
#include "../utils/util.h"

// Constants.
#define NUM_IDT_ENTRIES 256

// Variables.
isr_t interrupt_handlers[NUM_IDT_ENTRIES];		// An array of pointers to interrupt handlers.

// Private function declarations.
void handle_isr(interrupt_args_t r);
void handle_irq(interrupt_args_t r);

// Public functions.

/**
 * Adds a handler to the array of interrupt handlers.
 *
 * idx: The index of the interrupt for which we want to add a handler.
 * handler: A pointer to the handler function.
 */
void register_interrupt_handler(u8 idx, isr_t handler) {
    interrupt_handlers[idx] = handler;
}

// Private functions.

/**
 * An array of the message associated with each interrupt.
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
 * The IDT delegates the handling of ISRs to this function.
 *
 * r: The arguments pushed before handling the interrupt.
 */
void handle_isr(interrupt_args_t r) {
    print("Received an interrupt: ");
    char interrupt_number[3];
    int_to_ascii(r.interrupt_no, interrupt_number);
    print(interrupt_number);
    print("\n");
    print(exception_messages[r.interrupt_no]);
    print("\n");
}

/**
 * The IDT delegates the handling of IRQs to this function.
 *
 * r: The arguments pushed before handling the interrupt request.
 */
void handle_irq(interrupt_args_t r) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */
	if (r.interrupt_no >= 40) port_write_byte(0xA0, 0x20); /* slave */
    port_write_byte(0x20, 0x20); /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r.interrupt_no] != 0) {
    	isr_t handler = interrupt_handlers[r.interrupt_no];
        handler(r);
    }
}
