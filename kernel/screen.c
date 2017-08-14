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

/**
 * Prints a string at the cursor's current location.
 *
 * *str: The char array to print.
 */
void print(char *str) {
    print_at(str, -1, -1);
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

        // We calculate the new cursor location.
        location -= 2 * MAX_COLS;
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
 * Converts a column and row to the corresponding integer location.
 *
 * col: The column to convert.
 * row: The row to convert.
 *
 * returns: The corresponding integer location.
 */
int coords_to_loc(int col, int row) { 
    return 2 * (row * MAX_COLS + col); 
}

/**
 * Extracts the column from an integer location.
 *
 * location: The integer location to convert.
 *
 * returns: The corresponding screen column.
 */
int loc_to_col(int location) { 
    return (location - (loc_to_row(location) * 2 * MAX_COLS)) / 2; 
}

/**
 * Extracts the row from an integer location.
*
 * location: The integer location to convert.
 *
 * returns: The corresponding screen row.
 */
int loc_to_row(int location) { 
    return location / (2 * MAX_COLS); 
}