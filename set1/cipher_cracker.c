#include <stdio.h>
#include <stdlib.h>

#include "bitlib.h"
#include "stats.h"
#include "_stats.h" //a bit of a hack so i can create global static struct...otherwise have to always call method

struct distribution letter_distribution = {.numSymbols = 26,
                                                  .frequencies = (unsigned int[]){
                                                    8200, //a = 8.2% 8.2*1000 = 8200..adding extra precision where perhaps not specified
                                                    1500, //b
                                                    2800, //c
                                                    4300, //d
                                                    13000, //e
                                                    2200, //f
                                                    2000, //g
                                                    6100, //h
                                                    7000, //i
                                                    150, //j
                                                    770, //k
                                                    4000, //l
                                                    2400, //m
                                                    6700, //n
                                                    7500, //o
                                                    1900, //p
                                                    95, //q
                                                    6000, //r
                                                    6300, //s
                                                    9100, //t
                                                    2800, //u
                                                    980, //v
                                                    2400, //w
                                                    150, //x
                                                    2000, //y
                                                    74 //z
                                                }, 
                                                .totalObservations = 100000
                                                };


// if i got all these return this, should maybe be returning a struct
char *crack_xor(unsigned char *binary, int bin_len, double (*error_func)(struct distribution *dist1, struct distribution *dist2) ,unsigned char *ret_best_key, double *ret_best_error, char log) {
    double best_error = 110; //normalised mean error can't exceed 100
    unsigned char best_key = 0;

    // 0 is the identiy so may as well start from 1
    // if used unsigned char...wraps around and keeps looping
    for (unsigned int key = 1; key <= 0xFF; key++) {
        // printf("testing key: %d\n", key);
        unsigned char *decoded_binary = bin_xor_key(binary, bin_len, key, 0);
        char *decoded_str = bin2AlphaString(decoded_binary, bin_len);

        struct distribution *decoded_dist = generate_letter_distribution(decoded_str);

        // double error = normalised_mean_absolute_error(&letter_distribution, decoded_dist);
        double error = normalised_rmse(&letter_distribution, decoded_dist);

        if (log) printf("key: %u error: %f decoded: %s\n", key, error, transform_break(decoded_str));

        if (error < best_error) {
            best_key = key;
            best_error = error;
        }

        destroy_dist(decoded_dist);
        free(decoded_binary);
        free(decoded_str);
    }


    //assuming we found a best key and best error that was different to starting

    unsigned char *decoded_binary = bin_xor_key(binary, bin_len, best_key, 0);
    char *decoded_str = bin2AlphaString(decoded_binary, bin_len);

    // free(decoded_binary);

    *ret_best_key = best_key;
    *ret_best_error = best_error;

    return decoded_str;
}