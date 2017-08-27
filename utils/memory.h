#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"

// Public functions.
void memory_copy(const char *src, char *dest, int len);
void memory_set(u8 *dest, u8 val, int len);

#endif
