#ifndef OS_DMA_H
#define OS_DMA_H

/**
 * DMA is split into two sets of channels:
 * * Channels 1, 2 and 3 are for 8-bit transfers to ISA peripherals
 * * Channels 5, 6 and 7 are for 16-bit transfers to ISA peripherals
 *
 * The DMA ports for channels 0-3:
 */
enum DMA_PORTS {
    // The DMA can access any memory from here until the next 64k boundary.
    START_ADDRESS_CHANNEL_2 = 0X04,

    // Must be set to the transfer length in bytes minus 1.
    COUNT_CHANNEL_2 = 0X05,

    // Masks a channel while setting it up, and unmasks it afterwards.
    // Bits 0-1: Selects the channel.
    // Bit 2: 0/1 to clear/set mask.
    SINGLE_CHANNEL_MASK = 0x0A,

    // Sets the DMA's mode.
    // Bits 0-1: Selects the channel.
    // Bits 2-3: Selects the transfer type.
    // Bit 4: Whether to reset the address and count registers post-transfer.
    // Bit 5: Reverses the memory order of the data.
    // Bits 6-7: Selects the transfer mode.
    MODE = 0x0B,

    // Must be sent before any 16-bit transfer.
    FLIP_FLOP_RESET = 0x0C,

    // The additional page register byte per channel allows the DMA controller
    // to access 24 bits of memory instead of 16.
    EXTERNAL_PAGE_CHANNEL_2 = 0x81
};

#endif //OS_DMA_H