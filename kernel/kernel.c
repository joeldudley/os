#include "screen.h"
#include "util.h"

/** 
 * Demonstrates screen printing and scrolling.
 */
void main() {
    clear_screen();

    // Use up the available rows.
    int i = 0;
    for (i = 0; i < 24; i++) {
        char str[255];
        int_to_ascii(i, str);
        print_string(str, 0, i);
    }

    print_string("This text forces the kernel to scroll. R\now 0 will disappear. ", 60, 24);
    print_string("And with this text, the kernel will scroll again, and row 1 will disappear too!", -1, -1);
}