/**
 * Copies a char array.
 */
void char_array_copy(char *src, char *dest, int len) {
    for (int i = 0; i < len; i++) {
        *(dest + i) = *(src + i);
    }
}

/**
 * Converts an int to its ASCII equivalent.
 */
void int_to_ascii(int n, char str[]) {
    // Flip the int if it's negative.
    int is_neg = (n < 0);
    if (is_neg) n = -n;

    // Build the ASCII integer in reverse.
    int i = 0;
    do {
        // The ASCII integer code are in order following '0'.
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    // Append the minus sign (as necessary) and the terminating null char.
    if (is_neg) str[i++] = '-';
    str[i] = '\0';

    // TODO: implement "reverse".
}