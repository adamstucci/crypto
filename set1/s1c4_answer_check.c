#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitlib.h"
#include "stats.h"
#include "cipher_cracker.h"

#define MAX_LINE 1024

int main(int argc, char const *argv[])
{
    char buffer[MAX_LINE +1];
    unsigned char expected_key = 5;
    unsigned char found_key = 72;

    int best_expected_line = 0;
    int best_found_line = 0;

    while (fgets(buffer, MAX_LINE+1, stdin) != NULL) {
        chomp(buffer);

        // a bit wasteful because our crack function already created distributions we just freed
        int bin_len = 0;
        unsigned char *binary = hex2bin(buffer, &bin_len);
        unsigned char *expected_binary = bin_xor_key(binary, bin_len, expected_key, 0);
        unsigned char *found_binary = bin_xor_key(binary, bin_len, found_key, 0);

        free(binary);

        char *expected_str = bin2AlphaString(expected_binary, bin_len);
        char *found_str = bin2AlphaString(found_binary, bin_len);

        free(found_binary);
        free(expected_binary);

        struct distribution *expected_dist = generate_letter_distribution(expected_str);
        struct distribution *found_dist = generate_letter_distribution(found_str);
        double expected_error = normalised_mean_absolute_error(&letter_distribution, expected_dist);
        double found_error = normalised_mean_absolute_error(&letter_distribution, found_dist);

        destroy_dist(found_dist);
        destroy_dist(expected_dist);

        // printf("input: %s\texpected output: %s\tobserved output: %s\texpected error: %f\tobserverd error: %f\n", buffer, expected_str, found_str, expected_error, found_error);
        printf("expected error: %f\tobserverd error: %f\n", expected_error, found_error);

        free(expected_str);
        free(found_str);
    }

    // printf("jsdkjsdkjkdjkjskdj\n");
    // printf("%hhu\n", best_key);
    // printf("key: %hhu %s\n", best_key, best_decrypt);
    return 0;
}
