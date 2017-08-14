#ifndef SCREEN_H
#define SCREEN_H

// Text screen video memory for colour monitors starts at 0xB8000.
#define VIDEO_ADDRESS (char*) 0xb8000

// The size of the screen.
#define MAX_ROWS 25
#define MAX_COLS 80
#define SCREEN_SIZE (MAX_COLS * MAX_ROWS)
#define MAX_LOC (2 * SCREEN_SIZE)

// Text mode memory uses two bytes for every character:
//   * An ASCII code byte
//   * An attribute byte
// Here are two standard values for the attribute byte:
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

// The VGA controller has two registers:
//   * A data register where data is placed.
//   * An index register indicating the type of the data in the VGA data register.
#define VGA_IDX_PORT 0x3d4
#define VGA_DATA_PORT 0x3d5

// VGA index register indices for the cursor location:
#define CURSOR_LOCATION_REGISTER_H 14
#define CURSOR_LOCATION_REGISTER_L 15

void clear_screen();
void print_at(char *message, int col, int row);
void print(char *message);

#endif