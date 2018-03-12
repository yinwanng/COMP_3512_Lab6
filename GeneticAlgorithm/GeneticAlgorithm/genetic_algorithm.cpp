#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include "City.h"
#include "Tour.h"


#define POPULATION_SIZE 30
#define CITIES_IN_TOUR  20

#define SHUFFLES          32     
#define ASCII_OFFSET      65     
#define ITERATIONS        1000   
#define FITNESS_SCALER    10000.0
#define PARENT_POOL_SIZE  5   
#define MUTATION_RATE     0.15    
#define NUMBER_OF_PARENTS 2      
#define NUMBER_OF_ELITES  1     

using namespace std;
void shuffle_cities(vector<City*> &permutation);
void swap_cities(int index_one, int index_two, vector<City*> &permutation);
int determine_fitness(vector<Tour*> population, int population_size);
double get_tour_distance(Tour *city_list);
double get_distance_between_cities(City *a, City *b);
int main()
{
	/* Variables */
	int    i = 0, j = 0, iterations = 0;
	int    index_of_shortest_tour = 0;
	double best_distance = 0.0;
	double best_iteration_distance = 0.0;
	double improvement_factor = 0.3;

	srand(time(NULL));
	vector<City*> cities_to_visit;
	vector<Tour*> population;
	vector<Tour*> temporary_tour;


	for (int i = 0; i < CITIES_IN_TOUR; i++)
	{
		cities_to_visit.push_back(new City((char)(i + ASCII_OFFSET)));
	}

	for (int i = 0; i < CITIES_IN_TOUR; i++)
	{
		cout << cities_to_visit[i]->getName() << " ";
		cout << cities_to_visit[i]->get_X() << " ";
		cout << cities_to_visit[i]->get_Y() << endl;
	}

		/* Populates and then randomly shuffles the CITIES_IN_TOUR cities in
	each candidate tour of our population of POPULATION_SIZE routes (tours).  */
	for (unsigned int i = 0; i < POPULATION_SIZE; ++i) {
		for (unsigned int j = 0; j < CITIES_IN_TOUR; ++j) {
			population.push_back(new Tour(0.0));
			population[i]->setPermutation(cities_to_visit);
		}
		// ...and then randomly shuffles them.
		shuffle_cities(population[i]->permutation); // Randomizes order of cities in this tour
	}

	/* Determines the fitness of each tour, and notes which tour is the fittest.  The
	determine_fitness function returns the index of the fittest individual. */
	index_of_shortest_tour = determine_fitness(population, POPULATION_SIZE);

	/* Sets the base distance.  This is our starting point. We'll use the best
	distance from our initial random population of size POPULATION_SIZE */
	best_distance = FITNESS_SCALER / (population[index_of_shortest_tour])->fitness;
	printf("Shortest distance in initial population: %8.3f\n", best_distance);
	cout << "Shortest distance in initial population: " << fixed << setprecision(3) << best_distance << endl;




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
			temporary_tour = population;
			population[0] = population[index_of_shortest_tour];
			population[index_of_shortest_tour] = temporary_tour[0];
		}



	}






	system("pause");
	return 0;
}

/*
* Shuffles the cities in an array (tour) of cities
* PARAM:  tour contains a set of CITIES_IN_TOUR cities
* PRE:    NULL
* POST:   the cities in tour have been shuffled
* RETURN: VOID
*/
void shuffle_cities(vector<City*> &permutation)
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

void swap_cities(int index_one, int index_two, vector<City*> &permutation)
{
	swap(permutation[index_one], permutation[index_two]);
	return;
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
int determine_fitness(vector<Tour*> population, int population_size)
{
	int i = 0, index_of_shortest_tour = 0;
	double shortest_tour_in_population = (double)RAND_MAX; // Begin by assuming distance is enormous
	double candidate_distance = 0.0;

	for (i = 0; i < population_size; ++i) {
		candidate_distance = get_tour_distance(population[i]);
		//population[i].fitness = FITNESS_SCALER / candidate_distance;
		population[i]->setFitness(FITNESS_SCALER / candidate_distance);
		if (candidate_distance <= shortest_tour_in_population) {
			index_of_shortest_tour = i;
			shortest_tour_in_population = candidate_distance;
		}
	}
	return index_of_shortest_tour;
}

/*
* Returns the total distance traveled on the
* specified array (tour) of cities.
* PARAM:  tour contains a set of CITIES_IN_TOUR cities
* PRE:    NULL
* POST:   NULL
* RETURN: the total distance, a double
*/
double get_tour_distance(Tour *city_list)
{
	int i = 0;
	double distance = 0.0;
	for (i = 0; i < CITIES_IN_TOUR; ++i) {
		distance +=
			get_distance_between_cities                         // so we create a loop
			(city_list->permutation[i], city_list->permutation[(i + 1) % CITIES_IN_TOUR]);
	}
	return distance;
}

/*
* Returns the distance between the two specified cities.
* PARAM:  a struct city
* PARAM:  a struct city
* PRE:    NULL
* POST:   NULL
* RETURN: the total distance, a double
*/
double get_distance_between_cities(City *a, City *b)
{
	return sqrt(pow((double)(a->get_X() - b->get_X()), 2.0) +
		pow((double)(a->get_Y() - b->get_Y()), 2.0));
}