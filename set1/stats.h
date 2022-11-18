#ifndef STATS_H
#define STATS_H

struct distribution; //forward declaration
// shouldn't know about implementations details hopefully


struct distribution *new_empty_dist(unsigned int numSymbols);
struct distribution *new_dist(unsigned int *frequencies, unsigned int numSymbols, char copyFrequencies, unsigned int totalObservations);
void add_observation(struct distribution * dist, unsigned int symbol);
double mean_absolute_error(struct distribution *dist1, struct distribution *dist2);

/// @brief computes the mean absolute error of two distributions witht the same sized alphabet
/// @param dist1 distribution 1
/// @param dist2 distribution 2
/// @return the mean absolute error
double mean_absolute_error(struct distribution *dist1, struct distribution *dist2);
double normalised_mean_absolute_error(struct distribution *dist1, struct distribution *dist2);
double normalised_rmse(struct distribution *dist1, struct distribution *dist2);
void destroy_dist(struct distribution *dist);
struct distribution *generate_letter_distribution(char *str);

#endif
