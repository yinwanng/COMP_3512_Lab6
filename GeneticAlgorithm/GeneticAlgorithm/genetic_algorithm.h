#pragma once

/*
* This header file contains the structs and prototypes
* for some functions used to demonstrate a genetic
* algorithm.
*
* Your lab this week is to convert this to an OOP
* using C++.
*/

#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

/* Definitions and then prototypes here */
#define POPULATION_SIZE 30
#define CITIES_IN_TOUR  20

/* A city structure contains:
a) a name
b) x and y coordinates */
struct city {
	char name;
	int  x_coordinate;
	int  y_coordinate;
};

/* A tour structure contains:
a) a fitness rating
b) an array of CITIES_IN_TOUR cities */
struct tour {
	double fitness;
	struct city permutation[CITIES_IN_TOUR];
};

void shuffle_cities(struct city * permutation);

double get_distance_between_cities(struct city a, struct city b);

double get_tour_distance(struct tour city_list);

int determine_fitness(struct tour * population, int population_size);

void swap_cities(int index_one, int index_two, struct city * permutation);

struct tour * select_parents(struct tour * population);

struct tour * crossover(struct tour * parents);

void mutate(struct tour * population);

int contains_city(struct tour * candidate_tour, int length, struct city * candidate_city);

#endif