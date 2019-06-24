#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "bitlib.h"

#define BUF_SIZE 1024

int main(int argc, char **argv) {
	char buffer[BUF_SIZE];
	assert(fgets(buffer, BUF_SIZE, stdin) != NULL);
	int len = strlen(buffer);
	if (buffer[len-1] == '\n') buffer[len-1] = '\0';
	int bin_len = 0;
	unsigned char *bin = hex2bin(buffer, &bin_len);
	printf("%d\n", bin_len);
	//int len2 = strlen(bin);
	//printf("bin is %d chars long\n", len2);
	//printf("%d\n", bin[0]);
	//print_bin(bin);
	//printf("\n");
	char *six = bin2six(bin, bin_len);
	print_six(six);
	printf("\n");
}
