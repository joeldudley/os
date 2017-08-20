#include "types.h"

/**
 * Extracts the lower 16 bits of a 32-bit address.
 *
 * address: The 32-bit address.
 */
u16 low_16(u32 address) {
    return (u16) (address & 0xffff);
}

/**
 * Extracts the higher 16 bits of a 32-bit address.
 *
 * address: The 32-bit address.
 */
u16 high_16(u32 address) {
    return (u16) ((address >> 16) & 0xffff);
}