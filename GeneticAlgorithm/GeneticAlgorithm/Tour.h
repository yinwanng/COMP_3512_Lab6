#pragma once
#include <vector>
#include "City.h"
class Tour
{
public:
	Tour(double);
	~Tour();
	void setFitness(double);
	double getFitness() const;
	std::vector<City*> getPermutation() const;

//private:
	double fitness;
	std::vector<City*> permutation;
};
