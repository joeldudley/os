#include "timer.h"
#include "../utils/ports.h"
#include "../interrupts/handle_interrupts.h"

// Constants.
uint32_t current_tick = 0;
int pit_tick_rate = 1193180;
uint16_t pit_command_port = 0x43;
uint16_t pit_channel_0_data_port = 0x40;

// Private functions declarations.
void timer_interrupt_handling_function(interrupt_args_t *_);

// Public functions.

/**
 * Sets a handler for IRQ0s, and sets IRQ0s to be made at a specific frequency.
 *
 * divisor: At what fraction of the PIT tick rate to send IRQ0s.
 */
void init_timer(uint32_t divisor) {
    // Sets a handler for IRQ0s.
    add_interrupt_handling_function_to_array(IRQ0, timer_interrupt_handling_function);

    // Calculate the desired IRQ0 frequency.
    uint32_t irq0_rate = pit_tick_rate / divisor;
    uint8_t low_irq0_rate = (uint8_t) (irq0_rate & 0xFF);
    uint8_t high_irq0_rate = (uint8_t) ((irq0_rate >> 8) & 0xFF);

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
void timer_interrupt_handling_function(interrupt_args_t *_) {
	current_tick++;
}
