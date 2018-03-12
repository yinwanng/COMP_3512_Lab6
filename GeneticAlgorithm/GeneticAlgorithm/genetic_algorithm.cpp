#include <iostream>
#include <vector>
#include <ctime>
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

int main()
{
	srand(time(NULL));
	vector<City*> cities_to_visit;
	vector<Tour*> population;

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


	system("pause");
	return 0;
}

