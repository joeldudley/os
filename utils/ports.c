#include <stdint.h>

// Public functions.

/**
 * Writes a byte to the specified I/O port.
 * 
 * port: The port to write to.
 * data: The byte to write.
 */
void port_write_byte(uint16_t port, uint8_t data) {
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

/**
 * Reads a byte from the specified I/O port.
 * 
 * port: The port to read from.
 * 
 * returns: The byte that was read.
 */
uint8_t port_read_byte(uint16_t port) {
    uint8_t result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}