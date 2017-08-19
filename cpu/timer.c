#include "timer.h"
#include "../kernel/screen.h"
#include "../kernel/util.h"
#include "idt.h"

u32 tick = 0;

static void timer_callback(interrupt_registers_t regs) {
    tick++;
    print("Tick: ");

    char tick_ascii[256];
    int_to_ascii(tick, tick_ascii);
    print(tick_ascii);
    print("\n");
}

void init_timer(u32 freq) {
    /* Install the function we just wrote */
    register_interrupt_handler(irq0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u32 divisor = 1193180 / freq;
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);

    /* Send the command */
    port_write_byte(0x43, 0x36); /* Command port */
    port_write_byte(0x40, low);
    port_write_byte(0x40, high);
}
