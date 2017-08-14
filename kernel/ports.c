#include "../cpu/types.h"

// Functions for writing to and reading from I/O devices.

/**
 * Write a byte to the specified I/O port.
 * 
 * port: The port to write to.
 * data: The byte to write.
 */
void port_write_byte(u16 port, u8 data) {
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

/**
 * Write a word to the specified I/O port.
 * 
 * port: The port to write to.
 * data: The word to write.
 */
void port_write_word(u16 port, u16 data) {
    asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}

/**
 * Read a byte from the specified I/O port.
 * 
 * port: The port to read from.
 * 
 * returns: The byte that was read.
 */
u8 port_read_byte(u16 port) {
    u8 result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/**
 * Read a word from the specified I/O port.
 * 
 * port: The port to read from.
 * 
 * returns: The word that was read. 
 */
u16 port_read_word(u16 port) {
    u16 result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}