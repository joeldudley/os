#include "screen.h"
#include "ports.h"
#include "util.h"

/* Private functions. */
int get_cursor_loc();
void set_cursor_loc(int location);
int print_char(char c, int col, int row, char attr);
int coords_to_loc(int col, int row);
int loc_to_col(int location);
int loc_to_row(int location);

/**********************************************************
 * Screen API                                             *
 **********************************************************/

/**
 * Prints a string at the cursor's current location.
 */
void print(char *string) {
    print_at(string, -1, -1);
}

/**
 * Prints a string at the specified (col, row) location.
 * If col/row < 0, prints at the cursor's current location.
 */
void print_at(char *string, int col, int row) {
    int location;
    if (col >= 0 && row >= 0)
        location = coords_to_loc(col, row);
    else 
        // Use current cursor location if col/row are negative.
        location = get_cursor_loc();

    // Loop through the string and print each character.
    for (int i = 0; string[i] != 0; i++) {
        col = loc_to_col(location);
        row = loc_to_row(location);
        // Update the location on each iteration.
        location = print_char(string[i], col, row, WHITE_ON_BLACK);
    }
}

/**********************************************************
 * Private screen functions                               *
 **********************************************************/

/**
 * Prints a char at the specified (col, row) location, sets the cursor to the 
 * next location, and returns this location.
 * If col/row < 0, prints at the cursor's current location.
 * If attr == null, prints white on black.
 */
int print_char(char c, int col, int row, char attr) {
    char *vid_mem = VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    // Error control: print a red 'E' if the coords aren't right.
    if (col >= MAX_COLS || row >= MAX_ROWS) {
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

    // If the char is a new-line, print nothing. 
    if (c == '\n') {
        row = loc_to_row(location);
        location = coords_to_loc(0, row + 1);
    } else {
        vid_mem[location] = c;
        vid_mem[location + 1] = attr;
        location += 2;
    }

    // Scroll if the location exceeds the screen size.
    if (location >= MAX_LOC) {
        // We copy every row into the row above, deleting the first row.
        for (int i = 1; i < MAX_ROWS; i++) 
            char_array_copy(
                VIDEO_ADDRESS + coords_to_loc(0, i),
                VIDEO_ADDRESS + coords_to_loc(0, i - 1),
                MAX_COLS * 2);

        // We clear the last row.
        char *last_line = VIDEO_ADDRESS + coords_to_loc(0, MAX_ROWS - 1);
        for (int i = 0; i < MAX_COLS * 2; i++)
            last_line[i] = 0;

        // We move the cursor down onto the newly-blank line.
        location -= 2 * MAX_COLS;
    }

    set_cursor_loc(location);
    
    return location;
}

/**
 * Read the cursor's current location.
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
 * Set the cursor's location.
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
 * Clear the screen.
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
 * Converts a column and row to the corresponding integer location.
 */
int coords_to_loc(int col, int row) { 
    return 2 * (row * MAX_COLS + col); 
}

/**
 * Extracts the column from an integer location.
 */
int loc_to_col(int location) { 
    return (location - (loc_to_row(location) * 2 * MAX_COLS)) / 2; 
}

/**
 * Extracts the row from an integer location.
 */
int loc_to_row(int location) { 
    return location / (2 * MAX_COLS); 
}