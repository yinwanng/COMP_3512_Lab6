#include <iostream>
#include <ctime>
#include "City.h"

#define MAP_BOUNDARY      1000 

City::City(char name)
{
	this->name = name;
	x = rand() % (MAP_BOUNDARY + 1);
	y = rand() % (MAP_BOUNDARY + 1);
}

City::~City()
{
}

void City::setName(char name)
{
	this->name = name;
}

char City::getName() const
{
	return name;
}

int City::get_X() const
{
	return x;
}

int City::get_Y() const
{
	return y;
}
