#ifndef STRING_H
#define STRING_H

#include <stdint.h>

// Public functions.
void int_to_ascii(int n, char str[]);
void reverse(char str[]);
int strlen(const char str[]);
void append(char str[], char n);
void remove_last_char(char str[]);
int strcmp(char str1[], char str2[]);

#endif