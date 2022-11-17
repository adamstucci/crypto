#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "_stats.h"

struct distribution *new_empty_dist(unsigned int numSymbols) {
    struct distribution *new = malloc(sizeof(struct distribution));
    assert(new != NULL);

    new->numSymbols = numSymbols;
    new->totalObservations = 0;
    new ->frequencies = malloc(sizeof(unsigned int) * numSymbols);
    assert(new->frequencies != NULL);

    memset(new->frequencies, 0, numSymbols * sizeof(unsigned int)); //insecure probs doesn't matter cause buffer size is determined by user input so can't overflow from untrusted user input
    return new;
}

void destroy_dist(struct distribution *dist) {
    free(dist->frequencies);
    free(dist);
}

// maybe should always copy to make destroy easier/safer i.e don't want double free
struct distribution *new_dist(unsigned int *frequencies, unsigned int numSymbols, char copyFrequencies, unsigned int totalObservations) {
    struct distribution *new = malloc(sizeof(struct distribution));
    assert(new != NULL);

    new->numSymbols = numSymbols;

    // int *newFrequencies = frequencies; //added indirection

    if (copyFrequencies) {
        new->frequencies = malloc(sizeof(unsigned int) * numSymbols);
        assert(new->frequencies != NULL);

        for (int i = 0; i < numSymbols; i++) {
            new->frequencies[i] = frequencies[i];
        }
    }
    else {
        new->frequencies = frequencies;
    }

    if (totalObservations == 0) {
        for (int i = 0; i < numSymbols; i++) {
            totalObservations += new->frequencies[i];
        }
    }

    new->totalObservations = totalObservations;

    return new;
}

void add_observation(struct distribution * dist, unsigned int symbol) {
    dist->frequencies[symbol] += 1;
    dist->totalObservations += 1;
}

double normalised_mean_absolute_error(struct distribution *dist1, struct distribution *dist2) {
    double sum_of_abs = 0;
    unsigned int num_symbols = dist1->numSymbols; //dist1 and dist2 should have same numsymbols

    for (int i = 0; i < num_symbols; i++) {
        double percentage1 = (double) dist1->frequencies[i] / dist1->totalObservations;
        double percentage2 = (double) dist2->frequencies[i] / dist2->totalObservations;

        sum_of_abs += fabs(percentage1 - percentage2);
    }

    return sum_of_abs / num_symbols;
}

double mean_absolute_error(struct distribution *dist1, struct distribution *dist2) {
    unsigned int sum_of_abs = 0;
    unsigned int num_symbols = dist1->numSymbols; //dist1 and dist2 should have same numsymbols

    for (int i = 0; i < num_symbols; i++) {
        // note casting from out of range unsigned to signed is undefined
        // also the subtraction might not put us in the signed integer range
        // might be better to use ternary
                                                                          // doesn't actually work like this...or not guaranteed to work like this..would be buggy anyway if out of range
        // sum_of_abs += abs(dist1->frequencies[i] - dist2->frequencies[i]); //large unsigned -> negative int -> positive int -> stored in unsigned
        sum_of_abs += dist1->frequencies[i] > dist2->frequencies[i] ? dist1->frequencies[i] - dist2->frequencies[i] : dist2->frequencies[i] - dist1->frequencies[i];
    }

    return (double) sum_of_abs / num_symbols;
}

// double mean_root_error(struct distribution *dist1, struct distribution *dist2) {

// }

// using double to make as generic as possible but should work with ints...though our distributions are int only
// ok maybe make the arguments for the error function int...but return needs to be double for e.g msre
// double mean_error(struct distribution *dist1, struct distribution *dist2, double (*error_func)(int a, int b)) {

// }

// might want to make this separate from the adt and put in a separate file
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
