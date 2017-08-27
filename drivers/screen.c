#include "screen.h"

#include "../utils/types.h"
#include "../utils/ports.h"
#include "../utils/memory.h"

// Constants.

// Base address of text screen video memory for colour monitors.
#define VIDEO_ADDRESS (char*) 0xb8000

#define SCREEN_COLS 80
#define SCREEN_ROWS 25
#define SCREEN_SIZE (SCREEN_COLS * SCREEN_ROWS)
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

// Private function declarations.
int get_cursor_loc();
void set_cursor_loc(int location);
int print_char(char c, int col, int row, char attr);
int coords_to_loc(int col, int row);
int loc_to_col(int location);
int loc_to_row(int location);

// Public functions.

/**
 * Clears the screen.
 */
void clear_screen() {
    char *vid_mem = VIDEO_ADDRESS;

    // We overwrite each video cell and reset its colour.
    for (int i = 0; i < SCREEN_SIZE; i++) {
        vid_mem[i * 2] = ' ';
        vid_mem[i * 2 + 1] = WHITE_ON_BLACK;
    }

    // We move the cursor to the start of the screen.
    set_cursor_loc(coords_to_loc(0, 0));
}

/**
 * Prints a string at the specified (col, row) location.
 * If col/row < 0, prints at the cursor's current location.
 *
 * *str: The char array to print.
 * col: The screen column to start printing at.
 * row: The screen row to start printing at.
 */
void print_at(char *str, int col, int row) {
    int location;
    if (col >= 0 && row >= 0)
        location = coords_to_loc(col, row);
    else 
        // Use current cursor location if col/row are negative.
        location = get_cursor_loc();

    // Loop through the string and print each character.
    for (int i = 0; str[i] != 0; i++) {
        col = loc_to_col(location);
        row = loc_to_row(location);
        // Update the location on each iteration.
        location = print_char(str[i], col, row, WHITE_ON_BLACK);
    }
}

/**
 * Prints a string at the cursor's current location.
 *
 * *str: The char array to print.
 */
void print(char *str) {
    print_at(str, -1, -1);
}

/**
 * Deletes the character at the current screen location.
 */
void print_backspace() {
    int offset = get_cursor_loc() - 2;
    int row = loc_to_row(offset);
    int col = loc_to_col(offset);
    print_char(0x08, col, row, WHITE_ON_BLACK);
}

// Private functions.

/**
 * Prints a char at the specified (col, row) location, sets the cursor to the 
 * next location, and returns this location.
 * If col/row < 0, prints at the cursor's current location.
 * If attr == null, prints white on black.
 *
 * c: The char to print.
 * col: The screen column to print at.
 * row: The screen row to print at.
 * attr: The attribute byte for the char.
 *
 * returns: The cursor's new location.
 */
int print_char(char c, int col, int row, char attr) {
    char *vid_mem = VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    // Error control: print a red 'E' if the co-ordinates aren't right.
    if (col >= SCREEN_COLS || row >= SCREEN_ROWS) {
        vid_mem[MAX_LOC - 2] = 'E';
        vid_mem[MAX_LOC - 1] = RED_ON_WHITE;
        return coords_to_loc(col, row);
    }

    int location;
    if (col >= 0 && row >= 0)
        location = coords_to_loc(col, row);
    else
        // Use current cursor location if col/row are negative.
        location = get_cursor_loc();

    if (c == '\n') {
        // If the char is a new-line, print nothing and advance to the start of the next line.
        row = loc_to_row(location);
        location = coords_to_loc(0, row + 1);
    } else if (c == 0x08) {
        // If the char is a backspace, delete the character at the current screen location.
        vid_mem[location] = ' ';
        vid_mem[location + 1] = attr;
    } else {
        vid_mem[location] = c;
        vid_mem[location + 1] = attr;
        location += 2;
    }

    // Scroll if the location exceeds the screen size.
    if (location >= MAX_LOC) {
        // We copy every row into the row above, deleting the first row.
        for (int i = 1; i < SCREEN_ROWS; i++) 
            memory_copy(
                VIDEO_ADDRESS + coords_to_loc(0, i),
                VIDEO_ADDRESS + coords_to_loc(0, i - 1),
                SCREEN_COLS * 2);

        // We clear the last row.
        char *last_line = VIDEO_ADDRESS + coords_to_loc(0, SCREEN_ROWS - 1);
        for (int i = 0; i < SCREEN_COLS * 2; i++)
            last_line[i] = 0;

        // We calculate the new cursor location.
        location -= 2 * SCREEN_COLS;
    }

    // We move the cursor down onto the newly-blank line.
    set_cursor_loc(location);
    
    return location;
}

/**
 * Gets the cursor's current location.
 *
 * returns: The cursor's current location.
 */
int get_cursor_loc() {
    // Request the high byte of the cursor's location.
    port_write_byte(VGA_IDX_PORT, CURSOR_LOCATION_REGISTER_H);
    int current_cell = port_read_byte(VGA_DATA_PORT) << 8;
    // Request the low byte of the cursor's location.
    port_write_byte(VGA_IDX_PORT, CURSOR_LOCATION_REGISTER_L);
    current_cell += port_read_byte(VGA_DATA_PORT);
    // Each character cell is 2 bytes wide.
    return current_cell * 2;
}

/**
 * Sets the cursor's location.
 *
 * location: The cursor's new location.
 */
void set_cursor_loc(int location) {
    // Each character cell is 2 bytes wide.
    location /= 2;
    // Write the high byte of the cursor's location.
    port_write_byte(VGA_IDX_PORT, CURSOR_LOCATION_REGISTER_H);
    port_write_byte(VGA_DATA_PORT, (u8) (location >> 8));
    // Write the low byte of the cursor's location.
    port_write_byte(VGA_IDX_PORT, CURSOR_LOCATION_REGISTER_L);
    port_write_byte(VGA_DATA_PORT, (u8) (location & 0xff));
}

/**
 * Converts a column and row to the corresponding integer location.
 *
 * col: The column to convert.
 * row: The row to convert.
 *
 * returns: The corresponding integer location.
 */
int coords_to_loc(int col, int row) { 
    return 2 * (row * SCREEN_COLS + col); 
}

/**
 * Extracts the column from an integer location.
 *
 * location: The integer location to convert.
 *
 * returns: The corresponding screen column.
 */
int loc_to_col(int location) { 
    return (location - (loc_to_row(location) * 2 * SCREEN_COLS)) / 2; 
}

/**
 * Extracts the row from an integer location.
*
 * location: The integer location to convert.
 *
 * returns: The corresponding screen row.
 */
int loc_to_row(int location) { 
    return location / (2 * SCREEN_COLS); 
}