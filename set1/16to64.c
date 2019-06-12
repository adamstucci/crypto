#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000
#define TOP_SIX 0xfc
#define BOT_TWO 0x3
#define TOP_FOUR 0xf0
#define BOT_FOUR 0x0f
#define BOT_SIX 0xcf

char encode(char c);

//where len is the number of characters not including null terminator
//len not needed if null terminated

//0th index is start of string. would probs be easier if represented it backwards e.g for addition and stuff so least significant is  at 0th index
//assuming well formatted hex string
char *hex2bin(char *str) {
	int len = strlen(str);
	char *bin;
	char alt_flag = 0; //for if in high or low parts of the byte...furthest left is high order 0 means high order
	unsigned char mask = 0xf0;
	//mask for setting the new bits....mask not what I want???
	if (len % 2 == 0) {
		bin = malloc(sizeof(char) * (len/2 + 1)); //+1 for null terminator
	}
	else {
		bin = malloc(sizeof(char) * (len/2+2)); //+2 for null terminator and final half byte needs a full byte...div by 2 floored
		alt_flag = 1;
		mask = 0x0f;
	}
	char *curr = str;
	while (curr != '\0') {
		//to upper to make consistent
		if ('a' >= *curr && 'z' <= *curr) {
			*curr = *curr - 'a' + 'A';
		}
		
		curr++;
	}
}

//is len needed
//every 6 bits is converted to character
//assume string is high order to low order...but well formed so can just grab 6bits at a time
//truncate leading zeroes
//essentially assuming number of bits is a multiple of 48....that's at most 6 wasted bytes...
char *binTo64(char *str, int len) {
	char leading = 1;
	int state = 0;
	int i = 0;
	int j = 0;
	char *new = malloc(((sizeof(char) * len) / 6) + 1) //+1 for null terminator
	assert(new != NULL);
	unsigned char result = 0;
	while (i < len) {
		result = 0;
		switch(state) {
			case 0: //top 6
				result = TOP_SIX & str[i];
				result = result >> 2;
				break;
			case 1: //bottom 2
				result = BOT_TWO & str[i];
				result = result << 4;
				result |= ((TOP_FOUR & str[i+1]) >> 4); //hopefully doesn't cause signed issues
				i++;
				break;
			case 2: //bottom 4
				result = ((BOT_FOUR & str[i]) << 2);
				result |=  ((TOP_TWO & str[i+1]) >> 6);
				i++;
				break;
			case 3: //bottom 6
				result = BOT_SIX & str[i];
				i++;
				break;
		}
		state = (state + 1) % 4; //every iteration a new 6bits are read;
		if (leading && result == 0) continue;
		leading = 0;
		new[j] = encode(result);
		j++;
	}
	new[j] = '\0';
	return new;
}

char encode(char c) {
	if (c >= 0 && c < 26) return c + 'A';
	if (c >= 26 && c < 52) return c - 26 + 'a';
	if (c >= 52 && c < 62) return c - 52 + '0';
	if (c == 62) return '+';
	if (c == 63) return '/';
}

int main(int argc, char **argv) {
	char input[MAX_SIZE];
	input = fgets(input, MAX_SIZE, stdin);
	return EXIT_SUCCESS;
}
