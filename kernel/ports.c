#include "../cpu/types.h"

/** In GCC inline assembly:
    * 'a' refers to register EAX
    * 'b' refers to register EBX
    * 'c' refers to register ECX
    * 'd' refers to register EDX
    * 'S' refers to register ESI
    * 'D' refers to register EDI

    * 'in' reads from a port
    * 'out' writes to a port

    * The sections are separated by colons:
        * The middle section assigns the ASM value to the C variable in brackets
        * The final section passes the C variable in brackets to ASM
 */

/**
 * Write a byte to the specified port.
 */
void port_write_byte(u16 port, u8 data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

/**
 * Write a word to the specified port.
 */
void port_write_word(u16 port, u16 data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

/**
 * Read a byte from the specified port.
 */
u8 port_read_byte(u16 port) {
    u8 result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/**
 * Read a word from the specified port.
 */
u16 port_read_word(u16 port) {
    u16 result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}