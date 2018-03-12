#include "Tour.h"

Tour::Tour(double fitness)
{
	this->fitness = fitness;
}

Tour::~Tour()
{
}

void Tour::setFitness(double fitness)
{
	this->fitness = fitness;
}

double Tour::getFitness() const
{
	return fitness;
}

std::vector<City*> Tour::getPermutation() const
{
	return std::vector<City*>();
}

void Tour::setPermutation(std::vector<City*> permutation)
{
	this->permutation = permutation;
}
