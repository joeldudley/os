#include "floppy.h"
#include "../interrupts/handle_interrupts.h"
#include "screen.h"
#include "../utils/ports.h"
#include "../dma/dma.h"

// Private functions declarations.
void floppy_interrupt_handling_function(interrupt_args_t *_);

void dma_initialize();

// Public functions.

/**
 * Sets a handler for IRQ6s, and preps reading and writing to floppy disks.
 */
void init_floppy() {
    // Sets a handler for IRQ6s.
    add_interrupt_handling_function_to_array(IRQ6, floppy_interrupt_handling_function);

    // Initialises the DMA for floppy disk transfers.
    dma_initialize();

    // TODO: Reset the floppy drive.

    // TODO: Set driver information.
}

// Private functions.

/**
 * Called when IRQ6s are made.
 *
 * *_: Unused but required.
 */
void floppy_interrupt_handling_function(interrupt_args_t *_) {
    // TODO: Do stuff.
    print("floppy interrupt fired\n> ");
}

// TODO: Pass this as an argument into `dma_initialise`.
const int DMA_BUFFER = 0x2000;

/**
 * Initialises the DMA, by:
 * * Setting its start address
 * * Setting its external page register
 */
void dma_floppy_initialize() {
    // Masking channel 2.
    port_write_byte(SINGLE_CHANNEL_MASK, 0x06);

    // Reset flip-flop before transferring 16-bit value.
    port_write_byte(FLIP_FLOP_RESET, 0xFF);

    // Set the start address of channel 2 as 0x2000.
    // TODO: Is this ok? If we choose 0x1000, will that overwrite the kernel?
    port_write_byte(START_ADDRESS_CHANNEL_2, 0);
    port_write_byte(START_ADDRESS_CHANNEL_2, 0x20);

    // Reset flip-flop before transferring 16-bit value.
    port_write_byte(FLIP_FLOP_RESET, 0xFF);

    // Set the count register to 0x23FF.
    // 0x23FF = 512 (sector size) * 18 (sectors per track) - 1 = size of a 1.44mb floppy track.
    port_write_byte(COUNT_CHANNEL_2, 0xFF);
    port_write_byte(COUNT_CHANNEL_2, 0x23);

    // Setting the external page register to 0x0.
    // This gives us a full address of 0x002000.
    port_write_byte(EXTERNAL_PAGE_CHANNEL_2, 0);

    // Unmasking channel 2.
    port_write_byte(SINGLE_CHANNEL_MASK, 0x02);
}

/**
 * Tells the DMA to place the data the FDC sends it into the DMA's buffer.
 */
void dma_floppy_read() {
    // Masking channel 2.
    port_write_byte(SINGLE_CHANNEL_MASK, 0x06);

    // Single transfer, address increment, autoinit, read, channel 2.
    port_write_byte(MODE, 0x56);

    // Unmasking channel 2.
    port_write_byte(SINGLE_CHANNEL_MASK, 0x02);
}

/**
 * Tells the DMA to pass the data from its buffer to the FDC.
 */
void dma_floppy_write() {
    // Masking channel 2.
    port_write_byte(SINGLE_CHANNEL_MASK, 0x06);

    // Single transfer, address increment, autoinit, write, channel 2
    port_write_byte(MODE, 0x5A);

    // Unmasking channel 2.
    port_write_byte(SINGLE_CHANNEL_MASK, 0x02);
}

/**
 * Reads the first 128 bytes of sector 0 of the floppy disk.
 */
void read_floppy_sector_0() {
    uint8_t *sector = 0;

    print("\n\rSector 0 contents:\n\n\r");

    //! read sector from disk
    sector = floppy_seek_and_read_sector(0);

    //! display sector
    if (sector != 0) {
        for (int j = 0; j < 128; j++) {
            print("0x");
            print(sector[j]);
            print(" ");
        }
    } else
        print("\n\r*** Error reading sector from disk");

    print("\n\rDone.");
}

static uint8_t _CurrentDrive = 0;

const uint8_t FLOPPY_SECTORS_PER_TRACK = 18;

/**
 * Converts a linear block address into a head, track and sector.
 *
 * block: The block number to convert.
 * *head: Location where the converted head is stored.
 * *track: Location where the converted track is stored.
 * *sector: Location where the converted sector is stored.
 */
void flpydsk_lba_to_chs(int block, int *head, int *track, int *sector) {
    *head = block % (FLOPPY_SECTORS_PER_TRACK * 2) / FLOPPY_SECTORS_PER_TRACK;
    *track = block / (FLOPPY_SECTORS_PER_TRACK * 2);
    *sector = block % FLOPPY_SECTORS_PER_TRACK + 1;
}

//! turns the current floppy drives motor on/off
void flpydsk_control_motor(int bool) {
    //! select the correct mask based on current drive
    uint8_t motor = FDC_DOR_MASK_DRIVE0_MOTOR;

    //! turn on or off the motor of that drive
    if (bool)
        port_write_byte(FDC_DOR_REGISTER, _CurrentDrive | motor | FDC_DOR_MASK_RESET | FDC_DOR_MASK_DMA);
    else
        port_write_byte(FDC_DOR_REGISTER, FDC_DOR_MASK_RESET);

    //! in all cases; wait a little bit for the motor to spin up/turn off
    // TODO: Added an 'a' to remove annoying "import me now!" highlighting/
    sleepa(20);
}

/**
 * // TODO: What is this?
 */
uint8_t *floppy_seek_and_read_sector(int block) {
    //! convert LBA sector to CHS
    int head = 0, track = 0, sector = 1;
    flpydsk_lba_to_chs(block, &head, &track, &sector);

    //! turn motor on and seek to track
    flpydsk_control_motor(1);
    if (flpydsk_seek(track, head) != 0)
        return 0;

    //! read sector and turn motor off
    flpydsk_read_sector(head, track, sector);
    flpydsk_control_motor(0);

    //! warning: this is a bit hackish
    return (uint8_t *) DMA_BUFFER;
}

//! send command byte to fdc
void floppy_send_command(uint8_t cmd) {

    //! wait until data register is ready. We send commands to the data register
    for (int i = 0; i < 500; i++)
        if (flpydsk_read_status() & FDC_MSR_MASK_DATAREG)
            return port_write_byte(FDC_FIFO_REGISTER, cmd);
}

/**
 * Retrieves any error codes from Seek or Recalibrate commands.
 *
 * *st0: Location where the error code is stored.
 * *cyl: Location where the FDC's view of the current cylinder is stored.
 */
void flpydsk_check_interrupt(uint32_t *st0, uint32_t *cyl) {
    floppy_send_command(FDC_CMD_SENSE_INTERRUPT);

    // Either bit being set indicates an error (except post-reset with
    // polling).
    *st0 = floppy_read_data();

    // The FDC's view of what the current cylinder is.
    *cyl = floppy_read_data();
}

/**
 * // TODO: What is this?
 */
void flpydsk_read_sector(uint8_t head, uint8_t track, uint8_t sector) {

    uint32_t st0, cyl;

    //! set the DMA for read transfer
    dma_floppy_read();

    //! read in a sector
    floppy_send_command(
            FDC_CMD_READ_DATA | FDC_CMD_MASK_MULTITRACK |
            FDC_CMD_MASK_SKIP | FDC_CMD_MASK_DENSITY);
    floppy_send_command(head << 2 | _CurrentDrive);
    floppy_send_command(track);
    floppy_send_command(head);
    floppy_send_command(sector);
    // Corresponds to (2^2) * 128 = 512 bytes per sector.
    floppy_send_command(2);
    floppy_send_command(
            ((sector + 1) >= FLOPPY_SECTORS_PER_TRACK)
            ? FLOPPY_SECTORS_PER_TRACK : sector + 1);

    // The GAP 3 size between sectors.
    floppy_send_command(27);
    floppy_send_command(0xff);

    //! wait for irq
    flpydsk_wait_irq();

    //! read status info
    for (int j = 0; j < 7; j++)
        flpydsk_read_data();

    //! let FDC know we handled interrupt
    flpydsk_check_interrupt(&st0, &cyl);
}