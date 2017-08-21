#include "../interrupts/idt.h"
#include "../interrupts/handlers/keyboard.h"
#include "../interrupts/handlers/timer.h"

/** 
 * Testing interrupts.
 */
void main() {
    build_and_load_idt();

    asm volatile("sti");
//    init_timer(50);
    init_keyboard();
}
