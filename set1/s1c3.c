#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "bitlib.h"
#include "stats.h"

#define MAX_LINE


// for distribution purposes, ignore non alphabetical but decoding will decode properly
struct distribution *generate_letter_distribution(char *str) {
    struct distribution *dist = new_empty_dist(26);

    for (unsigned int i = 0; str[i] != '\0'; i++) {
        //only count character if alphabetic
        if (isalpha(str[i])) {
            unsigned int symbol = str[i] >= 'A' && str[i] <= 'Z' ? str[i] - 'A' : str[i] - 'a';
            add_observation(dist, symbol);
        }
    }

    return dist;
}

// use z test to score observed distribution, with known distribution of letters
// is this the best scoring metric
// root square mean error or something???
int main(int argc, char const *argv[])
{
    // wikipedia gives up to 3 decimal places
    // so times by 1000 so all percentages are integers
    // instead of out of 100, will be out of 100 000
    struct distribution *letter_distribution = new_dist((unsigned int[]){
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
    }, 26, 1, 100000); //do we actually need to copy here


    // is there an off by 1 error here???
    char buffer[MAX_LINE + 1]; //+1 for null terminator
    fgets(buffer, MAX_LINE + 1, stdin);
    chomp(buffer);

    int bin_len;
    unsigned char *binary = hex2bin(buffer, &bin_len);

    double best_error = 110; //normalised mean error can't exceed 100
    unsigned char best_key = 0;

    // 0 is the identiy so may as well start from 1
    // 
    for (unsigned char key = 1; key <= 0xFF; key++) {
        unsigned char *decoded_binary = bin_xor_key(binary, bin_len, key);
        char *decoded_str = bin2AlphaString(decoded_binary, bin_len);

        struct distribution *decoded_dist = generate_letter_distribution(decoded_str);

        double error = normalised_mean_absolute_error(letter_distribution, decoded_dist);

        if (error < best_error) {
            best_key = key;
            best_error = error;
        }

        destroy_dist(decoded_dist);
        free(decoded_binary);
        free(decoded_str);
    }

    //assuming we found a best key and best error that was different to starting

    unsigned char *decoded_binary = bin_xor_key(binary, bin_len, best_key);
    char *decoded_str = bin2AlphaString(decoded_binary, bin_len);

    printf("%s\n", decoded_str);


    free(decoded_str);
    free(decoded_binary);
    

    /* code */
    return 0;
}
