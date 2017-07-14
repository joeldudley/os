#include "screen.h"
#include "ports.h"

/* Declaration of private functions */
int get_cursor_loc();
void set_cursor_loc(int location);
int print_char(char c, int col, int row, char attr);
int get_loc_from_coords(int col, int row);
int get_row_from_loc(int location);
int get_col_from_loc(int location);

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

/**
 * Prints a string at the specified (col, row) location.
 * If col/row < 0, prints at the cursor's current location.
 */
void print_string(char *string, int col, int row) {
    int location;
    if (col >= 0 && row >= 0)
        location = get_loc_from_coords(col, row);
    else 
        // Use current cursor location if col/row are negative.
        location = get_cursor_loc();

    // Loop through the string and print it.
    int i = 0;
    while (string[i] != 0) {
        col = get_col_from_loc(location);
        row = get_row_from_loc(location);
        // Update the location on each iteration.
        location = print_char(string[i++], col, row, WHITE_ON_BLACK);
    }
}

/**********************************************************
 * Private kernel functions                               *
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
        int max_location = 2 * (MAX_COLS) * (MAX_ROWS);
        vid_mem[max_location - 2] = 'E';
        vid_mem[max_location - 1] = RED_ON_WHITE;
        return get_loc_from_coords(col, row);
    }

    int location;
    if (col >= 0 && row >= 0)
        location = get_loc_from_coords(col, row);
    else 
        // Use current cursor location if col/row are negative.
        location = get_cursor_loc();

    // If the char is a new-line, print nothing. 
    if (c == '\n') {
        row = get_row_from_loc(location);
        location = get_loc_from_coords(0, row + 1);
    } else {
        vid_mem[location] = c;
        vid_mem[location + 1] = attr;
        location += 2;
    }

    set_cursor_loc(location);

    return location;
}

/**
 * Read the cursor's current position.
 */
int get_cursor_loc() {
    // Request the high byte.
    port_write_byte(VGA_CTRL_REGISTER, 14);
    int location = port_read_byte(VGA_DATA_REGISTER) << 8;
    // Request the low byte.
    port_write_byte(VGA_CTRL_REGISTER, 15);
    location += port_read_byte(VGA_DATA_REGISTER);
    // Each character cell is 2 wide.
    return location * 2;
}

/**
 * Set the cursor's current position.
 */
void set_cursor_loc(int location) {
    // Each character cell is 2 wide.
    location /= 2;
    // Request the high byte.
    port_write_byte(VGA_CTRL_REGISTER, 14);
    port_write_byte(VGA_DATA_REGISTER, (unsigned char) (location >> 8));
    // Request the low byte.
    port_write_byte(VGA_CTRL_REGISTER, 15);
    port_write_byte(VGA_DATA_REGISTER, (unsigned char) (location & 0xff));
}

/**
 * Clear the screen's contents.
 */
void clear_screen() {
    char *vid_mem = VIDEO_ADDRESS;

    // We blank out each video cell and reset it's colour.
    for (int i = 0; i < SCREEN_SIZE; i++) {
        vid_mem[i * 2] = ' ';
        vid_mem[i * 2 + 1] = WHITE_ON_BLACK;
    }

    // We reset the cursor to the start of the screen.
    set_cursor_loc(get_loc_from_coords(0, 0));
}

int get_loc_from_coords(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_row_from_loc(int location) { return location / (2 * MAX_COLS); }
int get_col_from_loc(int location) { return (location - (get_row_from_loc(location) * 2 * MAX_COLS)) / 2; }