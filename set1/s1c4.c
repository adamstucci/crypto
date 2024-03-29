// answer is on line 171...if I pass in line 171 my decryptor can decrypt it
// but my decryptor may not be good enough to discern it from a bunch of other things
// I want to include non printable characters in my thing so the score can be even lower if predominantly non printable characters
// is it just tuning the scoring function...or is there a bigger bug

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
    char *best_decrypt = NULL;
    double best_error = 110;
    unsigned char best_key = 0;

    while (fgets(buffer, MAX_LINE+1, stdin) != NULL) {
        chomp(buffer);

        // a bit wasteful because our crack function already created distributions we just freed
        int bin_len = 0;
        unsigned char curr_key;
        double local_error;
        unsigned char *binary = hex2bin(buffer, &bin_len);
        char *curr_decrypt = crack_xor(binary, bin_len, normalised_rmse, &curr_key, &local_error, 0);
        struct distribution *curr_dist = generate_letter_distribution(curr_decrypt);
        double curr_error = normalised_rmse(&letter_distribution, curr_dist);

        chomp(curr_decrypt);
        printf("key: %hhu local error: %f global error: %f decryption: %s\n", curr_key, local_error, curr_error, transform_break(curr_decrypt));

        if (curr_error < best_error) {
            best_error = curr_error;
            if (best_decrypt != NULL) free(best_decrypt);
            best_decrypt = curr_decrypt;
            best_key = curr_key;
        }
        else {
            free(curr_decrypt);
        }

        destroy_dist(curr_dist);
        free(binary);
    }

    printf("answer: key: %hhu error: %f decryption: %s\n", best_key, best_error, transform_break(best_decrypt));
    // printf("%s\n", best_decrypt);
    // printf("jsdkjsdkjkdjkjskdj\n");
    // printf("%hhu\n", best_key);
    // printf("key: %hhu %s\n", best_key, best_decrypt);
    return 0;
}
