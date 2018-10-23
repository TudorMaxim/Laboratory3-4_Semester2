#include "tests.h"

void TestCountry() {
	country* r = CountryCreate("Romania", "Europe", 20000000);
	country* copy = CopyCountry(r);
	assert(r != copy);
	assert(strcmp(GetName(r), GetName(copy)) == 0);
	assert(strcmp(GetName(r), "Romania") == 0);
	assert(strcmp(GetContinent(r), "Europe") == 0);
	assert(GetPopulation(r) == 20000000);
	CountryDestroy(&copy);
	CountryDestroy(&r);
	assert(r == NULL);
}

void TestVector() {
	vector* V = VecCreate(1, &CountryDestroy);
	assert(V != NULL);

	country* r = CountryCreate("Romania", "Europe", 20000000);
	country* f = CountryCreate("France", "Europe", 30000000);
	country* g = CountryCreate("Germany", "Europe", 40000000);
	country *rr = CopyCountry(r);

	VecAdd(V, g);
	VecAdd(V, r);
	VecAdd(V, f);
	
	vector* W = VecFilter(V, "ma", &NamePatternMatchingCmp, &CopyCountry);
	country* el0 = VecGetElement(W, 0);
	country* el1 = VecGetElement(W, 1);

	assert(GetPopulation(el0) == GetPopulation(g));
	assert(GetPopulation(el1) == GetPopulation(r));
	VecDestroy(&W, &CountryDestroy);
	assert(W == NULL);

	VecDestroy(&V, &CountryDestroy);
	assert(V == NULL);
	CountryDestroy(&rr);

}

void TestRepository() {
	repository* repo = RepoCreate();
	assert(repo != NULL);

	country* r = CountryCreate("Romania", "Europe", 2000000);
	country* f = CountryCreate("France", "Europe", 3000000);
	country* g = CountryCreate("Germany", "Europe", 4000000);
	country* rr = CopyCountry(r);
	assert(RepoAdd(repo, r) == 0);
	assert(RepoAdd(repo, g) == 0);
	assert(RepoAdd(repo, f) == 0);
	assert(RepoAdd(repo, r) == 1);
	vector* V = RepoGetAll(repo);
	assert(V->elems[0] == r);
	assert(V->elems[1] == g);
	assert(V->elems[2] == f);
	assert(RepoFind(repo, "France") == 2);
	assert(RepoFind(repo, "Norway") == -1);
	assert(RepoDelete(repo, "Norway") == 1);
	assert(RepoDelete(repo, "Romania") == 0);
	assert(RepoUpdate(repo, "Germany", rr) == 0);
	assert(RepoMigrate(repo, 1000000, "France", "Romania") == 0);

	RepoDestroy(&repo);
	assert(repo == NULL);
}

void TestUtilities() {
	country* a = CountryCreate("Romania", "Europe", 100);
	country* b = CountryCreate("England", "Europe", 200);

	assert(NamePatternMatchingCmp(a, "Roma") == 1);
	assert(NamePatternMatchingCmp(a, "Bucharest") == 0);


	assert(IsNumber("1234") == 1);
	assert(IsNumber("Tudor") == 0);
	assert(IsNumber("0a34f") == 0);
	
	assert(ToInt("1234") == 1234);
	

	assert(ContinentPatternMatchingCmp(a, "Euro") == 1);
	assert(ContinentPatternMatchingCmp(a, "Africa") == 0);

	assert(AscendingByName(a, b) == 0);
	assert(AscendingByName(b, a) == 1);
	assert(DescendingByName(a, b) == 1);
	assert(DescendingByName(b, a) == 0);

	assert(AscendingByPopulation(a, b) == 1);
	assert(DescendingByPopulation(a, b) == 0);

	assert(PopulationMoreThan(a, 50) == 1);
	assert(PopulationMoreThan(a, 200) == 0);
	
	assert(FromContinent(a, "Europe") == 1);
	assert(FromContinent(a, "Asia") == 0);

	CountryDestroy(&a);
	CountryDestroy(&b);

	char* command = (char*)malloc(sizeof(char) * 30);
	strcpy(command, "add Romania Europe 20000");
	int argc = 0;
	char** argv = SplitCommand(command, &argc);
	assert(argc == 4);
	assert(strcmp(argv[0], "add") == 0);
	assert(strcmp(argv[1], "Romania") == 0);
	assert(strcmp(argv[2], "Europe") == 0);
	assert(strcmp(argv[3], "20000") == 0);
	free(command);
	for (int i = 0; i < argc; i++) {
		free(argv[i]);
	}
	free(argv);

}

void TestStack() {
	stack* S = StackCreate(&CountryDestroy);
	assert(S != NULL);
	country*r = CountryCreate("romania", "europe", 2000);
	country*f = CountryCreate("france", "europe", 3000);
	Push(S, r);
	Push(S, f);
	assert(GetTop(S) == f);
	Pop(S);
	assert(GetTop(S) == r);
	Pop(S);

	assert(GetTop(S) == -1);
	assert(IsEmpty(S) == 1);

	StackDestroy(&S, &CountryDestroy);
	assert(S == NULL);
}
