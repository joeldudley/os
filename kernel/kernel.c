#include "../interrupts/build_idt.h"
#include "../interrupts/handle_interrupts.h"
#include "../drivers/screen.h"

/** 
 * Testing interrupts.
 */
void main() {
    build_and_load_idt();
    initialise_hardware();
    clear_screen();
    print("Press ENTER to echo input. Input END to halt CPU.\n> ");
}