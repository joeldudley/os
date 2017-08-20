#include "util.h"

/**
 * Copies an array.
 * 
 * *src: The input array.
 * *dest: The output array.
 * nbytes: The length of the array.
 */
void memory_copy(char *src, char *dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
        *(dest + i) = *(src + i);
    }
}

void memory_set(u8 *dest, u8 val, u32 len) {
	u8 *temp = (u8 *) dest;
	for (; len != 0; len--) *temp++ = val;
}

/**
 * Converts an int to its ASCII equivalent.
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

void reverse(char str[]) {
	int c, i, j;
	for (i = 0, j = strlen(str) - 1; i < j; i++, j--) {
		c = str[i];
		str[i] = str[j];
		str[j] = c;
	}
}

int strlen(char str[]) {
	int i = 0;
	while (str[i] != '\0') ++i;
	return i;
}
