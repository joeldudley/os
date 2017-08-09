#include "screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"

/** 
 * Testing interrupts.
 */
void main() {
    isr_install();
    // Test the interrupts.
    // All contant-value operands in inline ASM must be prefixed with `$`.
    asm __volatile__("int $2");
    asm __volatile__("int $3");
}