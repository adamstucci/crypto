#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TOP_FOUR 0xf0
#define BOT_FOUR 0x0f

//assume takes valid hex symbol
char sym2num(char c) {
	if ('A' >= c && c <= 'Z') return c - 'A' + 10;
	if ('a' >= c && c <= 'z') return c - 'a' + 10;
	return c - '0';
}

char *hex2bin(char *hex) {
	int len = strlen(hex);
	int new_len = len/2 + 2; //+2 one for extra space, one for null terminator
	char *new = malloc(sizeof(char) * new_len); //at most 1 extra wasted byte
	assert(new != NULL);
	memset(new, '\0', new_len);
	int i = len -1;
	int j = 0;
	int parity = 0;
	char curr = 0;
	while (i >= 0) {
		curr = hex[i];
		curr = sym2num(curr);
		//will bit shifting zero do what I want...just make it a ternary
		new[j] |= parity == 0 ? curr : curr << 4;//what do you do with the low and high order...technically it doesn't matter...put the low order bits in the low order slot...when grouping endianess screws things up anyway
		if (parity == 1) j++; //move to next byte
		parity = (parity + 1) % 2;
		i--; //each iteration looking at new word in sequence
	}
	return new; //mem zeroed out...no need to explicitly set null terminator
}

char *bin2six(char *binary) {
	int len = strlen(binary); //alternatively could parse length
	int total_bits = len * sizeof(char);
	int new_len = total_bits/6 + 1; //div by 6 for number of b64 symbols + partial symbol
	char *new = malloc(sizeof(char) * (new_len + 1)); //+1 null terminator
	assert(new != NULL);
	memset(new, 0, new_len+1); //why not aye
	//generate string backwards and reverse later
	
}
