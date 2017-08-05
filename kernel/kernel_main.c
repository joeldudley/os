#include "screen.h"
#include "util.h"

/** 
 * Screen printing and scrolling.
 */
void main() {
    clear_screen();

    // Print on all the available rows.
    char str_to_print[255];
    for (int i = 0; i < 24; i++) {
        int_to_ascii(i, str_to_print);
        print_string(str_to_print, 0, i);
    }

    print_string("This text forces the kernel to scroll. Row 0 will disappear. ", 60, 24);
    print_string("And with this text, the kernel will scroll again, and row 1 will disappear too!", -1, -1);
}