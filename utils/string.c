#include "string.h"
#include <stdint.h>

// Public functions.

/**
 * Converts an int to its ASCII string equivalent.
 *
 * n: The integer to convert.
 * str[]: The output string.
 */
void int_to_ascii(int n, char str[]) {
    int is_neg = n < 0;

    // Take the absolute value of n.
    int pos_n;
    if (is_neg) {
        pos_n = -n;
    } else {
        pos_n = n;
    }

    // Build the ASCII integer in reverse.
    int str_idx = 0;
    do {
        // The ASCII integer codes are in order following '0'.
        str[str_idx++] = pos_n % 10 + '0';
    } while ((pos_n /= 10) > 0);

    // Append the minus sign if required, and the terminating null char.
    if (is_neg) {
        str[str_idx++] = '-';
    }
    str[str_idx] = '\0';

    // Reverse the ASCII integer's order.
    reverse(str);
}

/**
 * Converts a hex value to its ASCII string equivalent.
 *
 * hex: The hex value to convert.
 * str[]: The output string.
 * // TODO: Inspect this code to understand it.
 */
void hex_to_ascii(int hex, char str[]) {
    append(str, '0');
    append(str, 'x');
    char zeros = 0;

    int32_t tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (hex >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }

    tmp = hex & 0xF;
    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');
    else append(str, tmp + '0');
}

/**
 * Reverses the contents of a string.
 *
 * str[]: The string to reverse.
 */
void reverse(char str[]) {
	int current_char;
    int i, j;
	for (i = 0, j = strlen(str) - 1; i < j; i++, j--) {
		current_char = str[i];
		str[i] = str[j];
		str[j] = current_char;
	}
}

/**
 * Calculates the length of a string.
 *
 * str[]: The string for which to calculate the length.
 */
int strlen(const char str[]) {
	int i = 0;
	while (str[i] != '\0') {
        i++;
    }
	return i;
}

/**
 * Appends a char to a string.
 *
 * str[]: The string to append the char to.
 * c: The char to append.
 */
void append(char str[], char c) {
    int len = strlen(str);
    str[len] = c;
    str[len+1] = '\0';
}

/**
 * Removes the final char from a string.
 *
 * str[]: The string to remove the char from.
 */
void remove_last_char(char str[]) {
    int len = strlen(str);
    str[len-1] = '\0';
}

/**
 * Establishes which string is "greater", based on which string has the largest left-most
 * non-identical ASCII code.
 *
 * str1[]: The first string.
 * str2[]: The second string.
 */
int strcmp(char str1[], char str2[]) {
    int i;
    // Loop through the string while the contents are the same.
    for (i = 0; str1[i] == str2[i]; i++) {
        // If we loop until the end of the string, the strings are identical.
        if (str1[i] == '\0') return 0;
    }
    return str1[i] - str2[i];
}
