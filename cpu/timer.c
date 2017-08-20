#include "timer.h"

#include "../kernel/ports.h"
#include "../kernel/screen.h"
#include "../kernel/util.h"
#include "idt.h"
#include "types.h"

// Constants.
u32 current_tick = 0;

// Private functions.
void timer_callback(interrupt_registers_t regs);

// Public functions.

void init_timer(u32 freq) {
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u32 divisor = 1193180 / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);

    /* Send the command */
    port_write_byte(0x43, 0x36); /* Command port */
    port_write_byte(0x40, low);
    port_write_byte(0x40, high);
}

// Private functions.

void timer_callback(interrupt_registers_t regs) {
	current_tick++;

    char current_tick_ascii[256];
    int_to_ascii(current_tick, current_tick_ascii);

    print("Tick: ");
    print(current_tick_ascii);
    print("\n");
}
