#ifndef BITLIB_H
#define BITLIB_H

unsigned char *hex2bin(char *hex, int *bin_len);
char *bin2six(unsigned char *binary, int len);
void print_six(char *six);
void print_bin(char *bin);
#endif
