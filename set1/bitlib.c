#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//my masks were wrong
#define TOP_FOUR 0xf0
#define BOT_FOUR 0x0f
#define TOP_SIX 0xfc
#define BOT_SIX 0x3f
#define TOP_TWO 0xc0
#define BOT_TWO 0x03

//NEED TO CAST USE UNSIGNED WHERE POSSIBLE

//need to add if 0 is the only digit then allow zeroes
//could properly do it by counting bits instead of truncate 0...what if instead of numbers just want bit string with padded 0s
//my hack is wrong anyway..e.g ff will need 0s padded
//assume takes valid hex symbol
//prev soltn was to refactor binary so it is well formed...required some calculation and/or passing over thw string i.e round to a multiple of 6...why did I stop this
//alternatively count bytes and remainder and recognise need for padding
unsigned char sym2num(char c) {
	if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
	if (c >= 'a' && c <= 'z') return c - 'a' + 10;
	if (c >= '0' && c <= '9') return c - '0';
	return c;
}

char bin2sym64(unsigned char c) {
	if (c >= 0 && c < 26) return 'A' + c;
	if (c >= 26 && c < 52) return 'a' + c - 26;
	if (c >= 52 && c < 62) return '0' + c - 52;
	if (c == 62) return '+';
	if (c == 63) return '/';
	return c;
}

unsigned char *hex2bin(char *hex, int *bin_len) {
	int len = strlen(hex);
	//int new_len = len/2 + 2; //+2 one for extra space, one for null terminator
	int new_len = len/2 + (len % 2); //exactly how many array elements will be needed
	unsigned char *new = malloc(sizeof(unsigned char) * new_len); //at most 1 extra wasted byte
	assert(new != NULL);
	memset(new, 0, new_len);
	//the 'leading' 4 can be all zero but not the other group of 4

	int i = len -1; //least sig character in the hex string
	int j = 0; //least sig position in the binary array
	int parity = 0; //when 0 don't move to next byte, when 1 move to next byte

	// going from left to right but storing bytes in flipped order....doesn't really matter as long as decoded in the same way
	unsigned char curr = 0; //the current thing being looked at in the hex string...not really needed as separate variable
	while (i >= 0) { //while still processing characters
		curr = hex[i];
		curr = sym2num(curr); //takes the symbol and gives a number that can be fit in 4 bits
		//printf("%d\n", curr);
		//will bit shifting zero do what I want...just make it a ternary
		new[j] |= (parity == 0 ? curr : curr << 4);//what do you do with the low and high order...technically it doesn't matter...put the low order bits in the low order slot...when grouping endianess screws things up anyway
		if (parity == 1) j++; //move to next byte
		parity = (parity + 1) % 2;
		i--; //each iteration looking at new word in sequence
	}
	// if (parity == 1) j++;
	// *bin_len = j;
	//pretty sure bin_len will just be new_len
	*bin_len = new_len;
	return new; //mem zeroed out...no need to explicitly set null terminator
}

//still getting an off by 1 error
//can't use the null terminator in binary need to be aware of length
//need code to truncate leadding 0
//len in number of bytes
char *bin2six(unsigned char *binary, int len) {
	//calculating total bits wrong...sizeof is in bytes not bits
	int total_bits = len * sizeof(char) * 8;
	int new_len = total_bits/6 + 1; //div by 6 for number of b64 symbols + partial symbol
	char *new = malloc(sizeof(char) * (new_len + 1));
	assert(new != NULL);
	memset(new, 0, new_len+1); //why not aye
	//generate string backwards and reverse later
	int i = 0;
	int j = 0;
	int state = 0;
	unsigned char letter = 0;
	while (i < len) {
		switch(state) {
			case 0: //bottom 6
				letter = binary[i] & BOT_SIX;
				new[j] = bin2sym64(letter);
				j++;
				break;
			case 1: //top 2 bot 4
				letter = binary[i] & TOP_TWO;
				letter = letter >> 6; //will there be issues with sign
				i++;
				//padding case
				if (letter != 0 && i == len) {
					new[j] = bin2sym64(letter);
					j++;
					break;
				}
				//ignore leading 0
				else if (i == len) break;
				letter |= (binary[i] & BOT_FOUR) << 2; //does this work?
				new[j] = bin2sym64(letter);
				j++;
				break;
			case 2: //top 4 bot 2
				letter = binary[i] & TOP_FOUR;
				letter = letter >> 4;
				i++;
				if (letter != 0 && i == len) {
					new[j] = bin2sym64(letter);
					j++;
					break;
				}
				else if (i == len) break;
				letter |= (binary[i] & BOT_TWO) << 4;
				new[j] = bin2sym64(letter);
				j++;
				break;
			case 3: // top 6
				letter = binary[i] & TOP_SIX;
				letter = letter >> 2;
				i++;
				if (letter != 0 || i != len-1) {
					new[j] = bin2sym64(letter);
					j++;
				}
				break;
		}
		state = (state + 1) % 4;
	}
	return new;
}

//rather than convert to a base64 string...just print char by char...want both options
void print_six(char *six) {
	int len = strlen(six); //for some reason going to far, but these should all be zeroed out
	int i = len-1;
	while (i >= 0) {
		putchar(six[i]);
		i--;
	}
}

void print_bin(char *bin) {
	int len = strlen(bin);
	int i = len-1;
	while (i >= 0) {
		putchar(bin[i]);
		i--;
	}
}
