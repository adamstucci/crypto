#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "bitlib.h"

#define BUF_SIZE 1024

int main(int argc, char **argv) {
	char buffer[BUF_SIZE + 1]; //+1 for null terminator
	assert(fgets(buffer, BUF_SIZE+1, stdin) != NULL);
	int len = strlen(buffer);
	if (buffer[len-1] == '\n') buffer[len-1] = '\0'; //chomp
	int bin_len = 0;
	unsigned char *bin = hex2bin(buffer, &bin_len);
	char *six = bin2SixString(bin, bin_len);
	printf("%s\n", six);
}
