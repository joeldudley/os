#ifndef PORTS_H
#define PORTS_H

#include <stdint.h>

// Public functions.
void port_write_byte(uint16_t port, uint8_t data);
uint8_t port_read_byte(uint16_t port);

#endif