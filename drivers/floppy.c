#include "floppy.h"
#include "../interrupts/handle_interrupts.h"
#include "screen.h"
#include "../utils/ports.h"

// Private functions declarations.
void floppy_interrupt_handling_function(interrupt_args_t *_);

// Public functions.

/**
 * Sets a handler for IRQ0s, and sets IRQ0s to be made at a specific frequency.
 *
 * divisor: At what fraction of the PIT tick rate to send IRQ0s.
 */
void init_floppy() {
    // Sets a handler for IRQ6s.
    add_interrupt_handling_function_to_array(IRQ6, floppy_interrupt_handling_function);
    // Resets the floppy drive, triggering an IRQ6.
    reset_floppy();
}

void reset_floppy() {
    port_write_byte(0x3F2, 0);
    port_write_byte(0x3F2, 0xC);
}

// Private functions.

/**
 * Called when IRQ0s are made.
 *
 * _: Unused but required.
 */
void floppy_interrupt_handling_function(interrupt_args_t *_) {
    // TODO: Do stuff.
    print("floppy interrupt fired\n> ");
}

/**
 * DMA is split into two sets of channels:
 * * Channels 1, 2 and 3 are for 8-bit transfers to ISA peripherals
 * * Channels 5, 6 and 7 are for 16-bit transfers to ISA peripherals
 *
 * Channels 0-3 DMA ports:
 * * 0x04: Start address register channel 2 (write).
 *
 * * 0x05: Count register channel 2 (write).
 *   * Set this to the transfer length in bytes minus 1.
 *
 * * 0x0A: Single channel mask register (write).
 *   * Bits 0-1: Selects the channel.
 *   * Bit 2: 0/1 to clear/set mask.
 *   * We mask a channel while setting it up.
 *
 * * 0x0B: Mode register (write).
 *   * Bits 0-1: Selects the channel.
 *   * Bits 2-3: Selects the transfer type.
 *   * Bit 4: Whether to reset the address and count registers after a transfer.
 *   * Bit 5: Reverses the memory order of the data.
 *   * Bits 6-7: Selects the transfer mode.
 *
 * * 0x0C: Flip-flop reset register (write).
 *   * This command must be sent before any 16-bit transfer.
 *
 * // TODO: What is port 0x80 (external page register?)?
 */

/**
 * Initialises the DMA, with a buffer at 64k - 128k.
 */
void flpydsk_initialize_dma() {
    // Masking channel 2.
    port_write_byte(0x0a, 0x06);

    // Reset flip-flop before transferring 16-bit value.
    port_write_byte(0x0C, 0xff);

    // Set the start address of channel 2 as 0x20000.
    port_write_byte(0x04, 0);
    port_write_byte(0x04, 0x20);

    // Reset flip-flop before transferring 16-bit value.
    port_write_byte(0x0C, 0xff);

    // Set the count register to 0x23ff.
    // 0x23ff = 512 (sector size) * 18 (sectors per track) - 1 = size of a 1.44mb floppy track.
    port_write_byte(0x05, 0xff);
    port_write_byte(0x05, 0x23);

    // external page register = 0
    // TODO: Should this be 0x81?
    port_write_byte(0x80, 0);

    // Unmasking channel 2.
    port_write_byte(0x0a, 0x02);
}

//! prepare the DMA for read transfer
void flpydsk_dma_read() {
    // Masking channel 2.
    port_write_byte(0x0a, 0x06);

    port_write_byte(0x0b, 0x56); // single transfer, address increment, autoinit, read, channel 2
    // Unmasking channel 2.
    port_write_byte(0x0a, 0x02);
}

//! prepare the DMA for write transfer
void flpydsk_dma_write() {
    port_write_byte(0x0a, 0x06); // mask dma channel 2
    port_write_byte(0x0b, 0x5a); // single transfer, address increment, autoinit, write, channel 2
    port_write_byte(0x0a, 0x02); // unmask dma channel 2
}