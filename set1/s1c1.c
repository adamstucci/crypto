#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "bitlib.h"

// TODO original code sort of working but there is a buffer overflow going on i think
// can't believe i've wasted so much time today
// i used to be smarter
// don't need leading bit or anything...break up into cases
// if we start a 'token' in the last byte and it's zero then we no it's just padding
// to properly make the number of characters though in a single allocation at the beginning so can generate the string without
// reversing or special print need to know at what point would we start a token in the last one
// and when would it be 0...which is what the leading bit gives us
// maybe edit this code to add leading bit??

#define BUF_SIZE 1024

int main(int argc, char **argv) {
	char buffer[BUF_SIZE + 1]; //+1 for null terminator
	assert(fgets(buffer, BUF_SIZE+1, stdin) != NULL);
	int len = strlen(buffer);
	if (buffer[len-1] == '\n') buffer[len-1] = '\0'; //chomp
	int bin_len = 0;
	unsigned char *bin = hex2bin(buffer, &bin_len);
	// printf("%d\n", bin_len);
	//int len2 = strlen(bin);
	//printf("bin is %d chars long\n", len2);
	//printf("%d\n", bin[0]);
	//print_bin(bin);
	//printf("\n");
	// char *six = bin2six(bin, bin_len);
	// print_six(six);
	// printf("\n");
	char *six = bin2SixString(bin, bin_len);
	printf("%s\n", six);
}
