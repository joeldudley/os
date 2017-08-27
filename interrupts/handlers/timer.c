#include "timer.h"
#include "../../utils/ports.h"
#include "../handle_interrupts.h"

// Constants.
u32 current_tick = 0;
int pit_tick_rate = 1193180;
u16 pit_command_port = 0x43;
u16 pit_channel_0_data_port = 0x40;

// Private functions declarations.
void timer_callback(interrupt_args_t _);

// Public functions.

/**
 * Sets a handler for IRQ0s, and sets IRQ0s to be made at a specific frequency.
 *
 * divisor: At what fraction of the PIT tick rate to send IRQ0s.
 */
void init_timer(u32 divisor) {
    // Sets a handler for IRQ0s.
    register_interrupt_handler_function(IRQ0, timer_callback);

    // Calculate the desired IRQ0 frequency.
    u32 irq0_rate = pit_tick_rate / divisor;
    u8 low_irq0_rate = (u8) (irq0_rate & 0xFF);
    u8 high_irq0_rate = (u8) ((irq0_rate >> 8) & 0xFF);

    // Set the PIT to make IRQ0s at the desired frequency.
    port_write_byte(pit_command_port, 0x36);
    port_write_byte(pit_channel_0_data_port, low_irq0_rate);
    port_write_byte(pit_channel_0_data_port, high_irq0_rate);
}

// Private functions.

/**
 * Called when IRQ0s are made.
 *
 * _: Unused but required.
 */
void timer_callback(interrupt_args_t _) {
	current_tick++;
}
