#include "types.h"

// Public functions.

/**
 * Writes a byte to the specified I/O port.
 * 
 * port: The port to write to.
 * data: The byte to write.
 */
void port_write_byte(u16 port, u8 data) {
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

/**
 * Reads a byte from the specified I/O port.
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