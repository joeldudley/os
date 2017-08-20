#include "util.h"

// Public functions.

/**
 * Copies an array.
 * 
 * *src: The array to be copied.
 * *dest: The copied array.
 * len: The length of the array.
 */
void memory_copy(const char *src, char *dest, int len) {
    for (int i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
}

/**
 * Sets every element of an array to a value.
 *
 * *dest: The array to be updated.
 * val: The value to set every element of the array to.
 * len: The length of the array.
 */
void memory_set(u8 *dest, u8 val, int len) {
	u8 *temp = dest;
	for (; len != 0; len--) *temp++ = val;
}

/**
 * Converts an int to its ASCII representation.
 *
 * n: The integer to convert.
 * str[]: The output char array.
 */
void int_to_ascii(int n, char str[]) {
    int is_neg = n < 0;

    // Flip n's sign if n is negative.
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

    reverse(str);
}

/**
 * Reverses the contents of an array.
 *
 * str[]: The array to reverse.
 */
void reverse(char str[]) {
	char current_char;
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
 *
 * returns: The string's length.
 */
int strlen(const char str[]) {
	int i = 0;
	while (str[i] != '\0') ++i;
	return i;
}
