#ifndef _STATS_H
#define _STATS_H

struct distribution {
    unsigned int *frequencies; //counting occurences...don' need neg
    unsigned int numSymbols; //won't be negative
    unsigned int totalObservations; //for computing percentages
    unsigned int totalOutOfRange;
};

#endif