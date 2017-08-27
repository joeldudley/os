#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

// Public functions.
void memory_copy(const char *src, char *dest, int len);
void memory_set(uint8_t *dest, uint8_t val, int len);
uint16_t low_16(uint32_t address);
uint16_t high_16(uint32_t address);

#endif
