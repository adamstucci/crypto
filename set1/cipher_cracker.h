#ifndef CIPHER_CRACKER_H
#define CIPHER_CRACKER_H

#include "stats.h"

extern struct distribution letter_distribution;

char *crack_xor(unsigned char *binary, int bin_len, unsigned char *ret_best_key);

#endif