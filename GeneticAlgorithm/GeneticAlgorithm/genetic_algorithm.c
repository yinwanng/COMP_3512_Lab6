/*
* This source file contains the constants and
* functions used to drive a demonstrate of a
* genetic algorithm.
*
* Your lab this week is to convert this to an OOP
* using C++.
*/

/* Preprocessor directives */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "genetic_algorithm.h" // Declare 'home-made' headers last

/* Remember to avoid magic numbers by defining constants */
#define SHUFFLES          32      // A shuffle consists of swaps

#define ASCII_OFFSET      65      // if we cast an int with value 65
// as a char, we get 'A'

#define ITERATIONS        1000    // Number of loops for the algorithm

#define MAP_BOUNDARY      1000     // Size of the map (X and Y)

#define FITNESS_SCALER    10000.0 // Scales the fitness to move the
// decimal place to a reasonable spot

#define PARENT_POOL_SIZE  5       // Number of members randomly selected
// from the population, from which the
// fittest is made a 'parent'

#define MUTATION_RATE     0.15    // Chance of being mutated

#define NUMBER_OF_PARENTS 2       // Number of 'parents' for each cross

#define NUMBER_OF_ELITES  1       // The number of 'fittest' that remain
// uncrossed and unmutated into the next gen

/*
* Main function drives the program.
* PRE:       NULL (no pre-conditions)
* POST:      NULL (no side-effects)
* RETURN:    IF the program exits correctly
*            THEN 0 ELSE 1
*/
int main(void)
{
	/* Variables */
	int    i = 0, j = 0, iterations = 0;
	int    index_of_shortest_tour = 0;
	double best_distance = 0.0;
	double best_iteration_distance = 0.0;
	double improvement_factor = 0.3;

	/* Declares the pointers to our dynamically allocated memory. */
	struct tour * population;      // Holds our candidate population
	struct tour * parents;         // Used during crossover
	struct tour * crosses;         // Used during calculations
	struct tour * temporary_tour;  // Used during calculations
	struct city * cities_to_visit; // Stores master list of cities
	struct tour * child;

	/* Initializes dynamically allocated memory to specified sizes */
	population = (struct tour *) malloc(sizeof(struct tour) * POPULATION_SIZE);
	crosses = (struct tour *) malloc(sizeof(struct tour) * (POPULATION_SIZE - NUMBER_OF_ELITES));
	temporary_tour = (struct tour *) malloc(sizeof(struct tour));
	cities_to_visit = (struct city *) malloc(sizeof(struct city) * CITIES_IN_TOUR);
	parents = NULL;
	child = NULL;

	/* Declares and defines the master list of all the cities that we need to visit.
	The cities are named 'A', 'B', ... , and they each have a random X and Y
	coordinate */
	for (i = 0; i < CITIES_IN_TOUR; ++i) {
		(cities_to_visit + i)->name = (char)(i + ASCII_OFFSET); // ASCII math (== 'A', 'B', ...)
		(cities_to_visit + i)->x_coordinate = rand() % (MAP_BOUNDARY + 1); // 0 <= X <= MAP_BOUNDARY
		(cities_to_visit + i)->y_coordinate = rand() % (MAP_BOUNDARY + 1); // 0 <= Y <= MAP_BOUNDARY
	}

	/* Seeds the random number generator.  When we want to generate random numbers
	in C, we start by seeding the pseudo-random number generator.  In this case,
	we are feeding it the time function as the seed.  We almost always do this. */
	srand(time(NULL));

	/* Populates and then randomly shuffles the CITIES_IN_TOUR cities in
	each candidate tour of our population of POPULATION_SIZE routes (tours).  */
	for (i = 0; i < POPULATION_SIZE; ++i) {
		for (j = 0; j < CITIES_IN_TOUR; ++j) {

			// Copies the master list of city structs to each tour...
			(population + i)->permutation[j] = *(cities_to_visit + j);  // Order = 'A', 'B', 'C', ..., 'J'.
		}
		// ...and then randomly shuffles them.
		shuffle_cities((population + i)->permutation); // Randomizes order of cities in this tour

		(population + i)->fitness = 0.0; // Initiates all the fitnesses to 0
	}

	/* Determines the fitness of each tour, and notes which tour is the fittest.  The
	determine_fitness function returns the index of the fittest individual. */
	index_of_shortest_tour = determine_fitness(population, POPULATION_SIZE);

	/* Sets the base distance.  This is our starting point. We'll use the best
	distance from our initial random population of size POPULATION_SIZE */
	best_distance = FITNESS_SCALER / (population + index_of_shortest_tour)->fitness;
	printf("Shortest distance in initial population: %8.3f\n", best_distance);

	/* And now we loop until we find what we want--the population "evolves"!
	We can loop a certain number of times like:
	for (iterations = 0; iterations < ITERATIONS; ++iterations) { ... }

	OR

	We can loop until we observe a predetermined improvement:
	while ( best_distance / base_distance > improvement_factor) { ... } */
	for (iterations = 0; iterations < ITERATIONS; ++iterations) {

		/* 1. Selection: keep the best route by moving the fittest to the front of
		the population.  We won't  change it in this iteration, and we will
		call it an 'elite' individual) */
		if (index_of_shortest_tour != 0) {
			*temporary_tour = *population;
			*population = *(population + index_of_shortest_tour);
			*(population + index_of_shortest_tour) = *temporary_tour;
		}

		/* 2. Crossover (mix the rest of the routes and create new routes).  First
		we create a separate collection of POPULATION_SIZE - 1.  We store this
		in one of our utility buffers.  We choose parents by selecting the
		fittest tour from PARENT_POOL_SIZE randomly selected tours.  We do
		this NUMBER_OF_PARENTS times.  We cross the NUMBER_OF_PARENTS parents
		and store the result in our utility buffer
		*/
		for (j = 0; j < (POPULATION_SIZE - NUMBER_OF_ELITES); ++j) {
			parents = select_parents(population);
			child = crossover(parents);
			*(crosses + j) = *child;
			free(child);
			free(parents);
		}


		/* Then we copy the POPULATION_SIZE - NUMBER_OF_ELITES crosses from the utility buffer
		to our population array, making sure not to overwrite the elite tour */
		for (i = NUMBER_OF_ELITES; i < POPULATION_SIZE; ++i) {
			*(population + i) = *(crosses + (i - NUMBER_OF_ELITES));
			(population + i)->fitness = 0.0;
		}

		/* 3. Mutation (randomly mess a few up. excluding the elite tour[s]) */
		mutate(population);

		/* 4. Evaluation (assign each one a fitness aka FITNESS_SCALER * the inverse
		of the total distance traveled) */
		index_of_shortest_tour = determine_fitness(population, POPULATION_SIZE);

		/* At the end of this iteration, if we have made it further to our goal,
		we print some helpful information to standard output, and keep track */
		best_iteration_distance = get_tour_distance(population[index_of_shortest_tour]);
		if (best_iteration_distance < best_distance) {
			best_distance = best_iteration_distance;
			printf("New distance: %8.3f\n", best_iteration_distance);
		}
	}

	/* Prints summary information */
	printf("Shortest distance %8.3f\n",
		(FITNESS_SCALER / population[index_of_shortest_tour].fitness));

	/* And that's it! */
	system("pause");

	/* Frees all memory */
	free(temporary_tour);
	free(cities_to_visit);
	free(crosses);
	free(population);
	return 0;
}

/*
* Mutates the tour at the specified index of tours.
*
* Calculates a random mutation value for each city in the
* specified tour.  If this value < MUTATION_RATE, then the
* city is swapped with a randomly chosen city from the
* same array (tour).
*
* There could be better ways to mutate a tour of cities.
* Can you think of any?
*
* PARAM:  permutation, an array of struct tour
* PARAM:  int index
* PRE:    index is a valid index for permutation
* POST:   some of population's tours may have been mutated.
* RETURN: VOID
*/
void mutate(struct tour * population)
{
	int i = 0, j = 0, k = 0;
	double mutates = 0.0;
	for (i = 0 + NUMBER_OF_ELITES; i < POPULATION_SIZE; ++i) {
		for (j = 0; j < CITIES_IN_TOUR; ++j) {
			mutates = (double)rand() / (double)RAND_MAX;
			if (mutates <= MUTATION_RATE) {
				k = rand() % CITIES_IN_TOUR;
				swap_cities(j, k, (population + i)->permutation);
			}
		}
	}
}

/*
* Mixes the contents of the NUMBER_OF_PARENTS tours pointed to by
* the parents pointer, and returns a pointer to the mixture.
*
* How are we mixing them?  Not very well, yet.  This method has
* been hard-coded for 2 parents.  Can you see how we can extend the
* code to incorporate cities from > 2 parents?  Do we need any
* preconditions (i.e. NUMBER_OF_PARENTS < POPULATION )?
*
* We select a random index and use the cities from one parent
* to populate the mixed tour up to and including that index, and then
* the cities from the second parent to top up the tour, making sure we
* don't add cities that are already in the mixed tour.
*
* Can you think of ways to improve the way we cross the 'parents'?
*
* PARAM:  parents, a pointer to a struct tour
* PRE:    parents is a valid pointer to NUMBER_OF_PARENTS struct tours
* RETURN: a pointer to a struct tour combination of the parents
*/
struct tour * crossover(struct tour * parents)
{
	/* Variables */
	int i = 0;

	struct tour * child = (struct tour *) malloc(sizeof(struct tour));

	/* Here's how we use rand again.  We invoke the rand() function, and since
	we want the result to be between 0 and (CITIES_IN_TOUR - 1), we use the
	modulus operator */
	int boundary_index = rand() % CITIES_IN_TOUR;

	/* Sets the index of the mixed result to 0.0 */
	child->fitness = 0.0;

	/* Copies the first 'boundary_index' cities in order from parent 1 to the mixed
	result */
	for (i = 0; i < boundary_index; i++) {
		child->permutation[i] = parents->permutation[i];
	}

	/* Fills the rest of the cells with cities from parent 2 */
	while (boundary_index < CITIES_IN_TOUR) {
		for (i = 0; i < CITIES_IN_TOUR; ++i) {
			/* If the mixed tour doesn't already contain the city at that index in parent two... */
			if (!contains_city(child, boundary_index, &((parents + 1)->permutation[i]))) {

				/* ...then we add it from the second parent to the child... */
				child->permutation[boundary_index] = (parents + 1)->permutation[i];

				/* And increment the boundary_index */
				boundary_index++;
			}
		}
	}

	return child;
}

/*
* Returns 1 if the specified tour contains the specified city, else 0.
* PARAM:  pointer to a candidate_tour
* PARAM:  length of the candidate tour
* PARAM:  struct city, the city being sought
* PRE:    NULL
* POST:   NULL
* RETURN: IF candidate_tour CONTAINS city_name
*         THEN 1
*         ELSE 0
*/
int contains_city(struct tour * candidate_tour, int length, struct city * candidate_city)
{
	int i = 0;
	for (i = 0; i < length; ++i) {
		if (candidate_tour->permutation[i].name == (char)candidate_city->name &&
			candidate_tour->permutation[i].x_coordinate == (int)candidate_city->x_coordinate &&
			candidate_tour->permutation[i].y_coordinate == (int)candidate_city->y_coordinate) { // <------------
			return 1;
		}
	}
	return 0;
}

/*
* Selects NUMBER_OF_PARENTS parent tours.  Each parent
* is fittest of a subset of size PARENT_POOL_SIZE of the
* population, randomly selected,.
* PARAM:  pointer to a population of struct tour
* PRE:    PARENT_POOL_SIZE > POPULATION_SIZE
* POST:   NULL
* RETURN: pointer to a struct tour which points to
*/
struct tour * select_parents(struct tour * population)
{
	int i = 0, j = 0, k = 0, parent_index = 0;

	/* Chooses the best from PARENT_POOL randomly selected tours */
	struct tour * parents = (struct tour *) malloc(sizeof(struct tour) * NUMBER_OF_PARENTS);
	struct tour * parent_pool = (struct tour *) malloc(sizeof(struct tour) * PARENT_POOL_SIZE);

	for (i = 0; i < NUMBER_OF_PARENTS; ++i) {
		for (j = 0; j < PARENT_POOL_SIZE; ++j) {
			k = rand() % POPULATION_SIZE;
			*(parent_pool + j) = *(population + k);
		}
		parent_index = determine_fitness(parent_pool, PARENT_POOL_SIZE);
		*(parents + i) = *(parent_pool + parent_index);
	}
	free(parent_pool);
	return parents;

}

/*
* Calculates the fitness of each tour in the specifed population
* and stores it in its struct.  Also determines which tour has
* the best fitness.
* PARAM:  population an array of struct tour
* PARAM:  population_size of the array
* PRE:    population_size is the length of population
* POST:   the struct tour elements in the population array have
*         been updated with their correct fitness ratings
* RETURN: the index of the 'fittest' tour in the population array
*/
int determine_fitness(struct tour * population, int population_size)
{
	int i = 0, index_of_shortest_tour = 0;
	double shortest_tour_in_population = (double)RAND_MAX; // Begin by assuming distance is enormous
	double candidate_distance = 0.0;

	for (i = 0; i < population_size; ++i) {
		candidate_distance = get_tour_distance(population[i]);
		population[i].fitness = FITNESS_SCALER / candidate_distance;
		if (candidate_distance <= shortest_tour_in_population) {
			index_of_shortest_tour = i;
			shortest_tour_in_population = candidate_distance;
		}
	}

	return index_of_shortest_tour;
}

/*
* Shuffles the cities in an array (tour) of cities
* PARAM:  tour contains a set of CITIES_IN_TOUR cities
* PRE:    NULL
* POST:   the cities in tour have been shuffled
* RETURN: VOID
*/
void shuffle_cities(struct city * permutation)
{
	int i = 0, index_one = 0, index_two = 0;
	for (i = 0; i < SHUFFLES; ++i) {
		index_one = rand() % CITIES_IN_TOUR;
		index_two = rand() % CITIES_IN_TOUR;

		swap_cities(index_one, index_two, permutation);
	}
	return;
}

/*
* Swaps the cities at the specified indices in the
* specified array (tour) of cities.
* PARAM:  int index
* PARAM:  int index
* PARAM:  pointer to an array of struct city
* PRE:    both indices >= 0
* PRE :   both indices < CITIES_IN_TOUR
* POST:   the cities at the specified indices in the
*         specified tour have been swapped.
*/
void swap_cities(int index_one, int index_two, struct city * permutation)
{
	struct city * temp = (struct city *) malloc(sizeof(struct city));
	*temp = *(permutation + index_one);
	*(permutation + index_one) = *(permutation + index_two);
	*(permutation + index_two) = *temp;
	free(temp);
	return;
}

/*
* Returns the distance between the two specified cities.
* PARAM:  a struct city
* PARAM:  a struct city
* PRE:    NULL
* POST:   NULL
* RETURN: the total distance, a double
*/
double get_distance_between_cities(struct city a, struct city b)
{
	return sqrt(pow((double)(a.x_coordinate - b.x_coordinate), 2.0) +
		pow((double)(a.y_coordinate - b.y_coordinate), 2.0));
}

/*
* Returns the total distance traveled on the
* specified array (tour) of cities.
* PARAM:  tour contains a set of CITIES_IN_TOUR cities
* PRE:    NULL
* POST:   NULL
* RETURN: the total distance, a double
*/
double get_tour_distance(struct tour city_list)
{
	int i = 0;
	double distance = 0.0;
	for (i = 0; i < CITIES_IN_TOUR; ++i) {
		distance +=
			get_distance_between_cities                         // so we create a loop
			(city_list.permutation[i], city_list.permutation[(i + 1) % CITIES_IN_TOUR]);
	}
	return distance;
}