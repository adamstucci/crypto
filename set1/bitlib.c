#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bitlib.h" //for intellisense though maybe not needed

//my masks were wrong
#define TOP_FOUR 0xf0
#define BOT_FOUR 0x0f
#define TOP_SIX 0xfc
#define BOT_SIX 0x3f
#define TOP_TWO 0xc0
#define BOT_TWO 0x03

void chomp(char *str) {
	int len = strlen(str);
	if (str[len-1] == '\n') str[len-1] = '\0'; //chomp
}

unsigned char sym2num(char c) {
	if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
	if (c >= 'a' && c <= 'z') return c - 'a' + 10;
	if (c >= '0' && c <= '9') return c - '0';
	return c;
}

// assume input is a only a 4bit number basically
char bin2sym16(unsigned char c) {
	if (c <= 9) return '0' + c;
	// lowercase letters easier to spot in sea of numbers
	else return 'a' + c - 10;
}

char bin2sym64(unsigned char c) {
	if (c >= 0 && c < 26) return 'A' + c;
	if (c >= 26 && c < 52) return 'a' + c - 26;
	if (c >= 52 && c < 62) return '0' + c - 52;
	if (c == 62) return '+';
	if (c == 63) return '/';
	return c;
}

// should be equal sized
unsigned char *xor_bin(unsigned char * bin1, unsigned char * bin2, int bin_len) {
	unsigned char *xor_data = malloc(sizeof (unsigned char) * bin_len);

	for (int i = 0; i < bin_len; i++) {
		xor_data[i] = bin1[i] ^ bin2[i];
	}
	return xor_data;
}

// how will we know if we're xoring a padding bits or not...should probably keep track of a real size only
// from real size can easily figure out how many array elements there are from data type size???
unsigned char *bin_xor_key(unsigned char *bin, int bin_len, unsigned char key) {
	unsigned char *xored_bin = malloc(sizeof(unsigned char) * bin_len);
	assert(xored_bin != NULL);

	for (int i = 0; i < bin_len; i++) {
		xored_bin[i] = bin[i] ^ key;
	}
	return xored_bin;
}

// might be some issues with the very last character depending on padding
// for the purposes of challenge 3, each character should have been encoded as a 2 symbol long hex sequence, so shouldn't have padding, only explicit or implicit leading zero
// or weird results from xor.....treat all as real data
char *bin2AlphaString(unsigned char *binary, int len) {
	char *str = malloc(sizeof(char) * (len + 1)); //+1 for null terminator
	assert(str != NULL);
	str[len] = '\0';

	//data doesn't change, just putting it in proper order....is cast from unsigned char to char undefined if out of range??? I just want to treat the bits as a different type
	// maybe I should use a union for data punning as the middle man rather than direct cast....more well defined???
	// is union punning actually defined behaviour???

	for (int i = 0; i < len; i++) {
		str[len-1-i] = binary[i];
	}
	return str;
}

char *bin2HexString(unsigned char *binary, int len) {
	// at most the leading 4 bits are padding
	// double check decoding consistent with encoding
	int hex_str_len = len *2;
	unsigned char leading_byte = binary[len-1];

	// if the top four bits are zero, then this part is just padding
	if (!(leading_byte & TOP_FOUR)) hex_str_len--;

	char *hex_str = malloc(sizeof(char) * (hex_str_len + 1)); //+1 for null terminator
	assert(hex_str != NULL);
	hex_str[hex_str_len] = '\0';

	// starting from least significant in binary so want to start from end of string
	int j = hex_str_len-1;
	for (int i = 0; i < len -1; i++) {
		hex_str[j--] = bin2sym16(binary[i] & BOT_FOUR); //lower sig
		hex_str[j--] = bin2sym16((binary[i] & TOP_FOUR) >> 4); //higher sig
	}

	//handle the last byte special because depends if the'res padding
	// doing it out here so don't have to do an if statement each time in the loop...wasted branch
	hex_str[j--] = bin2sym16(binary[len -1] & BOT_FOUR);
	// would == 0 or >= 0 be a faster op????
	if (j >= 0) hex_str[j] = bin2sym16((binary[len-1] & TOP_FOUR) >> 4);

	return hex_str;
}

unsigned char *hex2bin(char *hex, int *bin_len) {
	int len = strlen(hex);
	int new_len = len/2 + (len % 2); //exactly how many array elements will be needed
	unsigned char *new = malloc(sizeof(unsigned char) * new_len); //at most 1 extra wasted byte
	assert(new != NULL);
	memset(new, 0, new_len);

	int i = len -1; //least sig character in the hex string
	int j = 0; //least sig position in the binary array
	int parity = 0; //when 0 don't move to next byte, when 1 move to next byte

	// going from left to right but storing bytes in flipped order....doesn't really matter as long as decoded in the same way
	unsigned char curr = 0; //the current thing being looked at in the hex string...not really needed as separate variable
	while (i >= 0) { //while still processing characters
		curr = hex[i];
		curr = sym2num(curr); //takes the symbol and gives a number that can be fit in 4 bits
		//will bit shifting zero do what I want...just make it a ternary...bit shift by 0 well defined
		new[j] |= (parity == 0 ? curr : curr << 4);//what do you do with the low and high order...technically it doesn't matter...put the low order bits in the low order slot...when grouping endianess screws things up anyway
		if (parity == 1) j++; //move to next byte
		parity = (parity + 1) % 2;
		i--; //each iteration looking at new word in sequence
	}

	*bin_len = new_len;
	return new; //mem zeroed out...no need to explicitly set null terminator
}

char *bin2six(unsigned char *binary, int len) {
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

char *bin2SixString(unsigned char * binary, int len) {
	//reverse the string...a  bit of a hack because extra work but to lazy right now and want the utility
	char *reversed_string = bin2six(binary, len);
	int six_len = strlen(reversed_string);
	char *six_string = malloc(sizeof(char) * (six_len + 1)); //+1 for null terminator
	assert(six_string != NULL);
	six_string[six_len] = '\0';

	for (int i = 0; i < six_len; i++) {
		six_string[i] = reversed_string[six_len -i -1];
	}

	free(reversed_string);
	return six_string;
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
