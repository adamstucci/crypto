#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "bitlib.h"
#include "stats.h"
#include "cipher_cracker.h"

#define MAX_LINE 1024

// use z test to score observed distribution, with known distribution of letters
// is this the best scoring metric
// root square mean error or something???
int main(int argc, char const *argv[])
{


    // is there an off by 1 error here???
    char buffer[MAX_LINE + 1]; //+1 for null terminator
    fgets(buffer, MAX_LINE + 1, stdin);
    chomp(buffer);

    int bin_len;
    unsigned char *binary = hex2bin(buffer, &bin_len);

    unsigned char key;
    double error;
    char *decrypted = crack_xor(binary, bin_len, &key, &error);

    printf("%s\n", decrypted);


    free(decrypted);
    free(binary);
    

    /* code */
    return 0;
}
