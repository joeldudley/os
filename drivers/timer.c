#include "timer.h"
#include "../interrupts/idt.h"
#include "../utils/ports.h"
#include "../utils/util.h"
#include "screen.h"

// Constants.
u32 current_tick = 0;
int clock_tick_rate = 1193180;

// Private functions declarations.
void timer_callback(interrupt_args_t _);

// Public functions.
// TODO: What is this doing?
void init_timer(u32 freq) {
    /* Install the function we just wrote */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u32 divisor = clock_tick_rate / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);

    /* Send the command */
    port_write_byte(0x43, 0x36); /* Command port */
    port_write_byte(0x40, low);
    port_write_byte(0x40, high);
}

// Private functions.
// TODO: What is this doing?
void timer_callback(interrupt_args_t _) {
	current_tick++;

    char current_tick_ascii[256];
    int_to_ascii(current_tick, current_tick_ascii);

    print("Tick: ");
    print(current_tick_ascii);
    print("\n");
}
