#ifndef PORTS_H
#define PORTS_H 

void port_write_byte(unsigned short port, unsigned char data);
void port_write_word(unsigned short port, unsigned short data);
unsigned char port_read_byte(unsigned short port);
unsigned short port_read_word(unsigned short port);

#endif