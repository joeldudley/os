#include "../utils/string.h"
#include "keyboard.h"
#include "screen.h"
#include "../utils/ports.h"
#include "../interrupts/handle_interrupts.h"

// Constants.
#define BACKSPACE 0x0E
#define ENTER 0x1C
#define RECOGNISED_SCANCODES 57

// Variables.
static char key_buffer[256];
const char *sc_name[] = {
        "ERROR", "Esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "Backspace",
        "Tab", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl", "A",
        "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", "LShift", "\\", "Z", "X", "C", "V",
        "B", "N", "M", ",", ".", "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii[] = {
        '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W',
        'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 'H',
        'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.',
        '/', '?', '?', '?', ' '};

// Private function declarations.
void keyboard_interrupt_handling_function(interrupt_args_t _);
void print_letter(uint8_t scancode);
void user_input(char *input);

// Public functions.

/**
 * Sets a handler for IRQ1s.
 */
void init_keyboard() {
    // Sets a handler for IRQ1s.
    add_interrupt_handling_function_to_array(IRQ1, keyboard_interrupt_handling_function);
}

// Private functions.

/**
 * Prints the string corresponding to a keyboard button press.
 *
 * _: Unused but required.
 */
void keyboard_interrupt_handling_function(interrupt_args_t _) {
    /* When a key is pressed, a scancode is placed in port 0x60. */
    uint8_t scancode = port_read_byte(0x60);

    if (scancode > RECOGNISED_SCANCODES) {
        return;
    }

    if (scancode == BACKSPACE) {
        remove_last_char(key_buffer);
        print_backspace();

    } else if (scancode == ENTER) {
        // Echoes the string we just typed.
        print("\n");

        if (strcmp(key_buffer, "END") == 0) {
            print("CPU halted.\n");
            asm volatile("hlt");

        } else {
            print(key_buffer);
            print("\n> ");

            key_buffer[0] = '\0';
        }

    } else {
        // Prints the key we pressed and adds it to the buffer.
        char letter = sc_ascii[(int) scancode];
        char str[2] = {letter, '\0'};
        print(str);
        append(key_buffer, letter);
    }
}