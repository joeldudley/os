#include "screen.h"
#include "util.h"
#include "../cpu/idt.h"

/** 
 * Testing interrupts.
 */
void main() {
    build_idt();
    // Test the interrupts.
    // All contant-value operands in inline ASM must be prefixed with `$`.
    asm volatile("int $2");
    asm volatile("int $3");
}