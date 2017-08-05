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
    int is_neg = n < 0;

    // Flip n's sign if n is negative.
    int pos_n;
    if (is_neg) {
        pos_n = -n;
    } else {
        pos_n = n;
    }

    // Build the ASCII integer in reverse.
    int str_loc = 0;
    do {
        // The ASCII integer code are in order following '0'.
        str[str_loc++] = pos_n % 10 + '0';
    } while ((pos_n /= 10) > 0);

    // Append the minus sign if required, and the terminating null char.
    if (is_neg) {
        str[str_loc++] = '-';
    }
    str[str_loc] = '\0';

    // TODO: Implement "reverse".
}