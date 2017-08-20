#ifndef TYPES_H
#define TYPES_H

// Used to allocate non-character bytes.
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

// Public functions.
u16 low_16(u32 address);
u16 high_16(u32 address);

#endif