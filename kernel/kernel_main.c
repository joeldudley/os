#include "screen.h"
#include "util.h"
#include "../cpu/idt.h"
#include "../cpu/timer.h"

/** 
 * Testing interrupts.
 */
void main() {
    build_and_load_idt();

    asm volatile("sti");
    init_timer(50);
    /* Comment out the timer IRQ handler to read
     * the keyboard IRQs easier */
//    init_keyboard();
}
