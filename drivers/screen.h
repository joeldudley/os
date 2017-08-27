#ifndef SCREEN_H
#define SCREEN_H

// Public functions.
void clear_screen();
void print_at(char *message, int col, int row);
void print(char *message);
void print_backspace();

#endif