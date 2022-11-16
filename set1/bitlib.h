#ifndef BITLIB_H
#define BITLIB_H

/// @brief 
/// @param hex string of hex characters with most sig character at front and least sig character at end 
/// @param bin_len how many elements are in the binary array
/// @return binary representation of hex value with least sig character at front and most sig character towards end...beware of padding
unsigned char *hex2bin(char *hex, int *bin_len);


char *bin2six(unsigned char *binary, int len);
void print_six(char *six);
void print_bin(char *bin);
#endif
