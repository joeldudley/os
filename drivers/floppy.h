#ifndef FLOPPY_H
#define FLOPPY_H

void init_floppy();

void reset_floppy();

/**
 * The FDC's four ports.
 */
enum FDC_PORTS {
    // The digital output register (DOR).
    // Controls the FDC.
    FDC_DOR_REGISTER = 0x3f2,

    // The main status register (MSR).
    // Provides the FDC's status.
    FDC_MSR_REGISTER = 0x3f4,

    // The data register.
    // Tells the FDC what actions to perform.
    FDC_FIFO_REGISTER = 0x3f5,

    // The configuration control register (CCR)/digital input register (DIR).
    FDC_CTRL_REGISTER = 0x3f7
};

/**
 * Masks for the DOR.
 */
enum FDC_DOR_MASK {
    // Bits 0-1: Selects the floppy disk drive.
    FDC_DOR_MASK_DRIVE0 = 0,
    FDC_DOR_MASK_DRIVE1 = 1,
    FDC_DOR_MASK_DRIVE2 = 2,
    FDC_DOR_MASK_DRIVE3 = 3,

    // Bit 2: Whether to reset the controller.
    FDC_DOR_MASK_RESET = 4,

    // Bit 3: Whether to use the FDC in IRQ or DMA mode.
    FDC_DOR_MASK_IRQ = 0,
    FDC_DOR_MASK_DMA = 8,

    // Bits 4-7: Whether to start the motor for each drive.
    FDC_DOR_MASK_DRIVE0_MOTOR = 16,
    FDC_DOR_MASK_DRIVE1_MOTOR = 32,
    FDC_DOR_MASK_DRIVE2_MOTOR = 64,
    FDC_DOR_MASK_DRIVE3_MOTOR = 128
};

/**
 * Masks for the MSR.
 */
enum FLOPPY_MSR_MASK {
    // Bits 0-3: Whether the floppy disk drives are currently seeking.
    FDC_MSR_MASK_DRIVE1_POS_MODE = 1,
    FDC_MSR_MASK_DRIVE2_POS_MODE = 2,
    FDC_MSR_MASK_DRIVE3_POS_MODE = 4,
    FDC_MSR_MASK_DRIVE4_POS_MODE = 8,

    // Bit 4: Set when a command byte is received, cleared at the end of the
    // Result phase.
    FDC_MSR_MASK_BUSY = 16,

    // Bit 5: Set during execution phase of PIO mode R/W commands.
    FDC_MSR_MASK_DMA = 32,

    // Bit 6: Set if FIFO I/O port expects an IN opcode.
    FDC_MSR_MASK_DATAIO = 64,

    // Bit 7: Set if it's OK to exchange bytes with the FIFO I/O port.
    FDC_MSR_MASK_DATAREG = 128
};

/**
 * Bits 0-4 of the command bytes that we will actually use to control the FDC.
 * There are also three "option bits" listed below.
 */
enum FDC_CMD {
    // Puts information in the FDC about the next disk to be accessed.
    FDC_CMD_SPECIFY = 3,

    // Read from the floppy.
    FDC_CMD_READ_DATA = 6,

    // Seek to cylinder zero.
    FDC_CMD_RECALIBRATE = 7,

    // Returns any error code from a Seek or Recalibrate command.
    FDC_CMD_SENSE_INTERRUPT = 8,

    // Seek both heads (?) to the specified cylinder.
    FDC_CMD_SEEK = 0xF
};

/**
 * The "option bits" to be masked with the command bytes above.
 */
enum FDC_CMD_MASK {
    // Leave this cleared.
    FDC_CMD_MASK_SKIP = 0x20,

    // Always set this for read/write/format/verify operations.
    FDC_CMD_MASK_DENSITY = 0x40,

    // Enables multi-track mode, allowing twice as much data to be read/
    // written with each command.
    FDC_CMD_MASK_MULTITRACK = 0x80
};

#endif