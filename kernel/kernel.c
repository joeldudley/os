#include "screen.h"

/** 
 * Writes various strings to the screen at various positions. 
 */
void main() {
    clear_screen();
    print_string("X", 1, 6);
    print_string("This text spans multiple lines", 75, 10);
    print_string("There is a line\nbreak", 0, 20);
    print_string("There is a line\nbreak", -1, -1);
    print_string("What happens when we run out of space?", 45, 24);
}