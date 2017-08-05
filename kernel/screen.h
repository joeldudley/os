#ifndef SCREEN_H
#define SCREEN_H

#define VIDEO_ADDRESS (char*) 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define SCREEN_SIZE (MAX_COLS * MAX_ROWS)
#define MAX_LOC (2 * SCREEN_SIZE)
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen I/O ports.
The VGA controller is split into two registers:
  * A data register which contains the data.
  * An index register indicating the type of the data in the VGA data register. */
#define VGA_IDX_PORT 0x3d4
#define VGA_DATA_PORT 0x3d5

/* Cursor location offsets in the VGA index register. */
#define CURSOR_LOCATION_REGISTER_H 14
#define CURSOR_LOCATION_REGISTER_L 15

/* Public kernel API. */
void clear_screen();
void print_string(char *message, int col, int row);

#endif