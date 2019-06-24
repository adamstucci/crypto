#include <stdio.h>

int main(void) {
	char c = '\0';
	c |= 1;
	printf("%d\n", c);
	char d = 1;
	d = d << 7;
	d = d >> 7;
	printf("%d\n", (unsigned char)d);
	return 0;
}
