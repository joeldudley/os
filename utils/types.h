#ifndef TYPES_H
#define TYPES_H

// Used to allocate non-character bytes.
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

// Used to extract the 16 low bytes of an address.
#define low_16(address) (u16) ((address) & 0xffff)

// Used to extract the 16 high bytes of an address.
#define high_16(address) (u16) (((address) >> 16) & 0xffff)

#endif