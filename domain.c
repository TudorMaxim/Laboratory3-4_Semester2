#include "domain.h"

country* CountryCreate(char* name, char* continent, int population) {
	country* c = (country*)malloc(sizeof(country));
	c->name = (char*)malloc(sizeof(char) * 1000);
	c->continent = (char*)malloc(sizeof(char) * 1000);
	strcpy(c->name, name);
	strcpy(c->continent, continent);
	c->population = population;
	return c;
}

void CountryDestroy(country** c) {
	if (*c == NULL) {
		return;
	}
	free((*c)->name);
	(*c)->name = NULL;
	
	free((*c)->continent);
	(*c)->continent = NULL;

	free(*c);
	*c = NULL;
}

char* GetName(country* c) {
	return c->name;
}

char* GetContinent(country* c) {
	return c->continent;
}

int GetPopulation(country* c) {
	return c->population;
}

country* CopyCountry(country* c) {
	country* a = CountryCreate(GetName(c), GetContinent(c), GetPopulation(c));
	return a;
}



