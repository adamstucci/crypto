#include <stdio.h>
#include <stdlib.h>

#include "bitlib.h"

#define MAX_LINE 1024

int main(int argc, char const *argv[])
{
    char buffer1[MAX_LINE + 1]; //on the stack...don't want extremely large stuff on the stack, especially in kernel code but that's something else
    char buffer2[MAX_LINE + 1];

    fgets(buffer1, MAX_LINE + 1, stdin);
    fgets(buffer2, MAX_LINE + 1, stdin);

    chomp(buffer1);
    chomp(buffer2);

    int bin_len1 = 0;
    int bin_len2 = 0;

    unsigned char *binary1 = hex2bin(buffer1, &bin_len1);
    unsigned char *binary2 = hex2bin(buffer2, &bin_len2);

    //these should end up with the same length

    //will differences in partial bits affect things...maybe...binary system not very robust
    // don't want to be XORing real data with padding....or when XORING...could produce new bits in the space that was padding, do we want this

    unsigned char *xored = xor_bin(binary1, binary2, bin_len1);

    // char *six_string = bin2SixString(xor_bin, bin_len1);
    // actually want hex string so padding probs won't matter..each symbol should have given 4 bits and should be equal number of symbols
    char *hex_string = bin2HexString(xored, bin_len1);
    printf("%s\n", hex_string);
    return 0;
}
