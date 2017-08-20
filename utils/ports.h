#ifndef PORTS_H
#define PORTS_H

#include "types.h"

// Public functions.
void port_write_byte(u16 port, u8 data);
u8 port_read_byte(u16 port);

#endif