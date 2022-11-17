#ifndef BITLIB_H
#define BITLIB_H

/// @brief 
/// @param hex string of hex characters with most sig character at front and least sig character at end 
/// @param bin_len how many elements are in the binary array
/// @return binary representation of hex value with least sig character at front and most sig character towards end...beware of padding
unsigned char *hex2bin(char *hex, int *bin_len);

char *bin2SixString(unsigned char * binary, int len);
// char *bin2six(unsigned char *binary, int len);
void print_six(char *six);
void print_bin(char *bin);
unsigned char *xor_bin(unsigned char * bin1, unsigned char * bin2, int bin_len);

/// @brief 
/// @param binary array of binary bottom 4 bytes of char is less significant then top 4 bytes of char
/// @param len number of elements in array
/// @return string in hex 
char *bin2HexString(unsigned char *binary, int len);

/// @brief chomps the given string in place
/// @param str 
void chomp(char *str);

unsigned char *bin_xor_key(unsigned char *bin, int bin_len, unsigned char key);
char *bin2AlphaString(unsigned char *binary, int len);
#endif
