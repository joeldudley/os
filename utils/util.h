#ifndef UTIL_H
#define UTIL_H

#include "types.h"

// Public functions.
void memory_copy(const char *src, char *dest, int len);
void memory_set(u8 *dest, u8 val, int len);
void int_to_ascii(int n, char str[]);
void reverse(char str[]);
int strlen(const char str[]);

#endif
