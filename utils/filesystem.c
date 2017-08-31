#include "memory.h"

// Public functions.

/**
 * Copies an array.
 * 
 * *src: The array to be copied.
 * *dest: The copied array.
 * len: The length of the array.
 */
void memory_copy(const char *src, char *dest, int len) {
    for (int i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
}

/**
 * Sets every element of an array to a value.
 *
 * *dest: The array to be updated.
 * val: The value to set every element of the array to.
 * len: The length of the array.
 */
void memory_set(uint8_t *dest, uint8_t val, int len) {
	uint8_t *temp = dest;
	for (; len != 0; len--) *temp++ = val;
}

/**
 * Extracts the lower 16 bits of a 32-bit address.
 *
 * address: The 32-bit address.
 */
uint16_t low_16(uint32_t address) {
    return (uint16_t) (address & 0xffff);
}

/**
 * Extracts the higher 16 bits of a 32-bit address.
 *
 * address: The 32-bit address.
 */
uint16_t high_16(uint32_t address) {
    return (uint16_t) ((address >> 16) & 0xffff);
}