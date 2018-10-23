#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>

typedef struct myCountry {
	char* name;
	char* continent;
	int population;
} country;

/*
Function that creates an entity of type country.
input: 
	name - a string
	continent - a string
	population - an integer number
output: 
	c - a new country
*/
country* CountryCreate(char* name, char* continent, int population);


/*
Function that destroys a country.
input: 
	c - a country
output: -
*/
void CountryDestroy(country** c);

//Copy a country
country* CopyCountry(country* c);

//Gets the name of a country
char* GetName(country* c);

//Gets the continent of a country
char* GetContinent(country* c);

//Gets the population of a country
int GetPopulation(country* c);

