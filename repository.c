#include "repository.h"

	
//Function that creates an empty repository.
repository* RepoCreate() {
	repository* repo  = (repository*)malloc(sizeof(repository));
	if (repo == NULL) {
		return NULL; //EXIT_FAILURE
	}
	repo->V = VecCreate(8, &CountryDestroy);
	return repo;
}

//Function tha destroys a repository
void RepoDestroy(repository** repo) {
	VecDestroy(&((*repo)->V), &CountryDestroy);
	free(*repo);
	*repo = NULL;
}

//Function that return all the elements from the repository
vector* RepoGetAll(repository* repo) {
	return repo->V;
}

/*
	Function that find a country in  the repository
	input: repo - the repository
		   name - a string containning the name of the searched country
	output: - the position on which we find the country with the given name in the repository
			- -1 if the country does not exist
*/
int RepoFind(repository* repo, char* name) {
	vector* W = RepoGetAll(repo);
	for (int i = 0; i < VecGetSize(W); i++) {
		country* elem = VecGetElement(W, i);
		if (stricmp(GetName(elem), name) == 0) {
			return i;
		}
	}
	return -1;
}

/*
	Function that adds an element to repository.
	input: repo - the initial repository
		   c - the country that is added
	output: repo - the new repository with the c country added
*/
int RepoAdd(repository* repo, country* c) {
	int pos = RepoFind(repo, GetName(c));
	if (pos != -1) {
		return 1; //The country already exists
	}
	VecAdd(repo->V, c);
	return 0; //EXIT_SUCCESS
}

/*
	Function that deletes an element from repository.
	input: repo - the initial repository
		   name - the name of the country which is deleted
	output: repo - the new repository without the country with the given name
*/
int RepoDelete(repository* repo, char* name) {
	int pos = RepoFind(repo, name);
	if (pos == -1) {
		return 1; //The country does not exist
	}
	VecDelete(repo->V, pos);
	return 0; //EXIT_SUCCESS
}

/*
	Function that updates a country existing in the repository
	input: repo - the initial repository
		   name - the name of the country which is updated - string
		   c - the new country - country*
	output: repo - the repository with the given country updated
*/
int RepoUpdate(repository* repo, char* name, country* c) {
	int pos = RepoFind(repo, name);
	if (pos == -1) {
		return 1; // The country does not exit
	}
	VecUpdate(repo->V, pos, c);
	return 0; //EXIT_SUCCESS
}

/*
	Function that updates the 2 countries in case of migration
	input: repo - the initial repository
		   number - the number of people that migrate - integer
		   from - the name of the country from which people migrate - string
		   to - the name of the country to which people migrate - string
	output: repo - the updated repository
*/
int RepoMigrate(repository* repo, int number, char* from, char* to) {
	int posFrom = RepoFind(repo, from);
	if (posFrom == -1) {
		return 1; // People cannot migrate from a country that does not exist
	}
	int posTo = RepoFind(repo, to);
	if (posTo == -1) {
		return 2; // People cannot migrate to a country that does not exist
	}
	if (posTo == posFrom) {
		return 3; //People cannot migrate from a country to the same country
	}
	country* f = VecGetElement(repo->V, posFrom);
	country* t = VecGetElement(repo->V, posTo);
	if (GetPopulation(f) < number) {
		return 4; // Cannot migrate more people from a country than the population of the country.
	}
	f->population -= number;
	t->population += number;
	country* cf = CopyCountry(f);
	country* ct = CopyCountry(t);
	VecUpdate(repo->V, posFrom, cf);
	VecUpdate(repo->V, posTo, ct);
	return 0; //EXIT_SUUCESS
}

/*
	Function that filters the countries from the repository.
	input: repo - the repository
		   pattern - the argument after which whe filter - genericType
		   cmp - the comapre function - CompareFunctionType
		   copy - the copy function for the elements from the repository - CopyFunctionType
	output: RepoFilter - a filtered vector with elements of genericType

*/
vector* RepoFilter(repository* repo, genericType pattern, CompareFunctionType cmp, CopyFunctionType copy) {
	return VecFilter(repo->V, pattern, cmp, copy);
}


operation* OperationCreate(char* name, int number, country* c1, country* c2) {
	operation* Op = (operation*)malloc(sizeof(operation));
	Op->number = number;
	Op->operationName = (char*)malloc(sizeof(char) * 100);
	strcpy(Op->operationName, name);
	Op->c1 = CopyCountry(c1);
	Op->c2 = CopyCountry(c2);
	return Op;
}

void OperationDestroy(operation** Op) {
	free((*Op)->operationName);
	(*Op)->number = 0;
	CountryDestroy(&((*Op)->c1));
	CountryDestroy(&((*Op)->c1));
	free(*Op);
	*Op = NULL;
}

operation* OperationCopy(operation* op) {
	operation* copy = OperationCreate(op->operationName, op->number, op->c1, op->c2);
	return copy;
}
