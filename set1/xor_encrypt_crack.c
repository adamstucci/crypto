#include <stdio.h>
#include <stdlib.h>

#include "stats.h"
#include "bitlib.h"

#define MAX_LINE 1024

int main(int argc, char const *argv[])
{
    char buffer[MAX_LINE + 1]; //+1 null terminator
    printf("enter a string to encrypt: ");
    fgets(buffer, MAX_LINE+1, stdin);

    unsigned char key;
    printf("enter an encrytpion key: ");
    
    
    return 0;
}
