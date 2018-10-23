#include "utilities.h"

char** SplitCommand(char* command, int *argc) {
	char** argv = (char**)malloc(sizeof(char*) * 10);
	int i = 0;
	for (char* p = strtok(command, " ,?!"); p != NULL; p = strtok(NULL, " ,!?")) {
		argv[i] = (char*)malloc(sizeof(char) * (strlen(p) + 5));
		strcpy(argv[i], p);
		i++;
	}
	*argc = i;
	return argv;
}

int IsNumber(char *s) {
	for (int i = 0; i < strlen(s); i++) {
		if (s[i] < '0' || s[i] > '9') return 0;
	}
	return 1;
}

int ToInt(char *s) {
	int val = 0;
	for (int i = 0; i < strlen(s); i++) {
		val = val * 10 + (s[i] - '0');
	}
	return val;
}

int NamePatternMatchingCmp(country* c, char* pattern) {
	if (strstr(GetName(c), pattern)) return 1;
	return 0;
}

int ContinentPatternMatchingCmp(country* c, char* pattern) {
	if (strstr(GetContinent(c), pattern)) return 1;
	return 0;
}

int AscendingByPopulation(country* a, country* b) {
	if (GetPopulation(a) < GetPopulation(b)) return 1;
	return 0;
}

int AscendingByName(country* a, country* b) {
	if (strcmp(GetName(a), GetName(b)) < 0) return 1;
	return 0;
}

int DescendingByName(country* a, country* b) {
	if (strcmp(GetName(a), GetName(b)) > 0) return 1;
	return 0;
}

int FromContinent(country*a, char* continent) {
	if (strcmp(GetContinent(a), continent) == 0) return 1;
	return 0;
}

int DescendingByPopulation(country* a, country* b) {
	if (GetPopulation(a) > GetPopulation(b)) return 1;
	return 0;
 }

int PopulationMoreThan(country* c, int p) {
	if (GetPopulation(c) > p) return 1;
	return 0;
}