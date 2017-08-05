#include "../cpu/types.h"

/** In GCC extended inline assembly:
    * The sections are separated by colons:
        * The first section is the instruction
            * Registers are denoted by '%', a character that we escape by writing "%%"
            * 'out %%1 %%2' writes the contents of register 1 to port 2
            * 'in %%1 %%2' stores the contents of port 1 in register 2
        * The second section assigns the register's value to the C variable in brackets
        * The third section assigns the value of the C variable in brackets to the register
        * In the second and third sections:
            * 'a' refers to registers %eax, %ax, and %al
            * 'b' refers to registers %ebx, %bx, and %bl
            * 'c' refers to registers %ecx, %cx, and %cl
            * 'd' refers to registers %edx, %dx, and %dl
            * 'S' refers to registers %esi and %si
            * 'D' refers to registers %edi and %di
 */

/**
 * Write a byte to the specified I/O port.
 */
void port_write_byte(u16 port, u8 data) {
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

/**
 * Write a word to the specified I/O port.
 */
void port_write_word(u16 port, u16 data) {
    asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}

/**
 * Read a byte from the specified I/O port.
 */
u8 port_read_byte(u16 port) {
    u8 result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/**
 * Read a word from the specified I/O port.
 */
u16 port_read_word(u16 port) {
    u16 result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}