#pragma once

#include "domain.h"


char** SplitCommand(char* command, int *argc);
int IsNumber(char *s);
int ToInt(char *s);

int AscendingByPopulation(country* a, country* b);
int DescendingByPopulation(country* a, country* b);
int AscendingByName(country* a, country* b);
int DescendingByName(country* a, country* b);
int FromContinent(country*a, char* continent);
int NamePatternMatchingCmp(country* x, char* pattern);
int ContinentPatternMatchingCmp(country* c, char* pattern);
int PopulationMoreThan(country* c, int p);