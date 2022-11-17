#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stats.h"
#include "bitlib.h"
#include "cipher_cracker.h"

#define MAX_LINE 1024

int main(int argc, char const *argv[])
{
    char buffer[MAX_LINE + 1]; //+1 null terminator
    printf("enter a string to encrypt: ");
    fgets(buffer, MAX_LINE+1, stdin);
    chomp(buffer);

    unsigned char key;
    printf("enter an encrytpion key: ");
    scanf("%hhu", &key);
    // printf("key is %hhu\n", key);

    int bin_len = strlen(buffer);
    unsigned char* encrypted_binary = bin_xor_key((unsigned char *) buffer, bin_len, key); //returns new encrypted binary

    //this won't work properly because my binary library expects least significant at the front...I should make it consistent with c strings
    // char *encrypted_hex_str = bin2HexString

    char *decrypted_str = crack_xor(encrypted_binary, bin_len);

    printf("most likely decryption: %s\n", decrypted_str);
    
    return 0;
}
