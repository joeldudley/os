#include "ports.h"

/** 
 * Writes 'X' to the cursor's current position. 
 */
void main() {
    int cursor_pos;
    // We request the high byte of the cursor (14) from the VGA control 
    // register (0x3d4).
    port_byte_out(0x3d4, 14);
    // We receive the high byte of the cursor via the VGA data register 
    // (0x3d5).
    cursor_pos = port_byte_in(0x3d5);

    // We left-shift this value because it's the high byte.
    cursor_pos = cursor_pos << 8;

    // We repeat the process for the low byte of the cursor (15).
    port_byte_out(0x3d4, 15);
    cursor_pos += port_byte_in(0x3d5);

    // Each VGA 'cell' pairs the character and its control data (e.g. 
    // background and foreground color). We double the cursor's position to 
    // find the VGA cell's location.
    int offset_from_vga = cursor_pos * 2;

    // We write 'X' to the current cursor position.
    char *vga = 0xb8000;
    vga[offset_from_vga] = 'X';
    // White text on a black background.
    vga[offset_from_vga+1] = 0x0f;
}