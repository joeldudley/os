#include "screen.h"
#include "util.h"

/** 
 * Testing interrupts.
 */
void main() {
    isr_install();
    // Test the interrupts.
    // You need to prefix all contant-value operands with `$`.
    asm __volatile__("int $2");
    asm __volatile__("int $3");
}