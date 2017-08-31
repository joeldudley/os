#include "../interrupts/build_idt.h"
#include "../interrupts/handle_interrupts.h"
#include "../drivers/screen.h"

/** 
 * Running the echo terminal.
 */
void kernel_main() {
    clear_screen();
    build_and_load_idt();
    initialise_irqs();
    print("Press ENTER to echo input. Input END to halt CPU.\n> ");
}