#include "../interrupts/idt.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"

/** 
 * Testing interrupts.
 */
void main() {
    build_and_load_idt();

    asm volatile("sti");
//    init_timer(50);
    init_keyboard();
}
