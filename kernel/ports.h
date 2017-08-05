#ifndef PORTS_H
#define PORTS_H

#include "../cpu/types.h"

void port_write_byte(u16 port, u8 data);
void port_write_word(u16 port, u16 data);
u8 port_read_byte(u16 port);
u16 port_read_word(u16 port);

#endif