#ifndef CIPHER_CRACKER_H
#define CIPHER_CRACKER_H

#include "stats.h"

extern struct distribution letter_distribution;

char* crack_xor(unsigned char* binary, int bin_len, double (*error_func)(struct distribution* dist1, struct distribution* dist2), unsigned char* ret_best_key, double* ret_best_error, char log);

#endif