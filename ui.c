#include "ui.h"

static void Instructions() {
	printf("Your options are: \n");
	printf("exit \n");
	printf("list \n");
	printf("add <name> <continent> <population> \n");
	printf("delete <name> \n");
	printf("update <name> in <new_name> <new_continent> <new_population> \n");
	printf("migrate <number> from <from_country_name> to <to_country_name> \n");
	printf("filter by name with <pattern>  \n"); //b
	printf("filter by continent with <pattern> \n"); // b bonus
	printf("filter ascending/descending by name from <continent>  \n"); // lab3 activity
	printf("filter descending by population having in name <pattern> \n"); // lab4 activity
	printf("filter ascending/descending  by population with more than <number> from <continent> \n"); // c + bonus
	printf("undo \n");
	printf("redo \n");
}

static void UiPrintMenu() {
	printf("Wellcome to my World Population Monitoring Application! \n");
	Instructions();
}

static interface* CreateInterface() {
	interface* I = (interface*)malloc(sizeof(interface));
	I->C = CtrlCreate();
	return I;
}

static void DestroyInterface(interface** I) {
	CtrlDestroy(&((*I)->C));
	free(*I);
	*I = NULL;
}

static void SetUp(interface* I) {
	country* argentina = CountryCreate("Argentina", "South-America", 20000);
	country* brasil = CountryCreate("Brazil", "South-America", 40000);
	country* usa = CountryCreate("Usa", "North-America", 150000);
	country* nz = CountryCreate("New-Zeeland", "Australia", 4000);
	country* nigeria = CountryCreate("Nigeria", "Africa", 70000);
	country* botswana = CountryCreate("Botswana", "Africa", 70000);
	country* greece = CountryCreate("Greece", "Europe", 50000);
	country* sweden = CountryCreate("Sweden", "Europe", 45000);
	country* afg = CountryCreate("Afghanistan", "Asia", 15045);
	country* japan = CountryCreate("Japan", "Asia", 90000);
	RepoAdd(I->C->repo, argentina);
	RepoAdd(I->C->repo, brasil);
	RepoAdd(I->C->repo, usa);
	RepoAdd(I->C->repo, nz);
	RepoAdd(I->C->repo, nigeria);
	RepoAdd(I->C->repo, botswana);
	RepoAdd(I->C->repo, greece);
	RepoAdd(I->C->repo, sweden);
	RepoAdd(I->C->repo, afg);
	RepoAdd(I->C->repo, japan);
}

static vector* GetAll(interface* I) {
	return CtrlGetAll(I->C);
}

static void UiPrintAll(interface* I) {
	vector* V = GetAll(I);
	if (VecGetSize(V) == 0) {
		printf("There are no countries registered! \n");
		return;
	}
	printf("The registered countries are: \n");
	for (int i = 0; i < VecGetSize(V); i++) {
		country* c = VecGetElement(V, i);
		printf("%s %s %d \n", GetName(c), GetContinent(c), GetPopulation(c));
	}
}

static void UiAdd(interface* I, int argc, char** argv) {
	if (argc != 4) {
		printf("Ivalid command! Read again the instructions! \n");
		Instructions();
		return;
	}
	if (IsNumber(argv[3]) == 0) {
		printf("The population must be a natural number! \n");
		return;
	}

	country* c = CountryCreate(argv[1], argv[2], ToInt(argv[3]));
	int exitCode = CtrlAdd(I->C, c);
	if (exitCode == 1) {
		printf("The country already exists! \n");
	} 
}

static void UiDelete(interface* I, int argc, char** argv) {
	if (argc != 2) {
		printf("Ivalid command! Read again the instructions! \n");
		Instructions();
		return;
	}
	int exitCode = CtrlDelete(I->C, argv[1]);
	if (exitCode == 1) {
		printf("The country does not exist! \n");
	}
}

static void UiUpdate(interface* I, int argc, char** argv) {
	if (argc != 6) {
		printf("Ivalid command! Read again the instructions! \n");
		Instructions();
		return;
	}
	if (IsNumber(argv[5]) == 0) {
		printf("The population must be a natural number! \n");
		return;
	}
	country* c = CountryCreate(argv[3], argv[4], ToInt(argv[5]));
	int exitCode = CtrlUpdate(I->C, argv[1], c);
	if (exitCode == 1) {
		printf("The country does not exist! \n");
	}
}

static void UiMigrate(interface* I, int argc, char** argv) {
	if (argc != 6) {
		printf("Ivalid command! Read again the instructions! \n");
		Instructions();
		return;
	}
	int exitCode = CtrlMigrate(I->C, ToInt(argv[1]), argv[3], argv[5]);

	if (exitCode == 1) {
		printf("People cannot migrate from a country that does not exist! \n");
	}
	else if (exitCode == 2) {
		printf("People cannot migrate to a country that does not exist!\n");
	}
	else if (exitCode == 3) {
		printf("People cannot migrate from a country to the same country! \n");
	}
	else if (exitCode == 4) {
		printf("Cannot migrate more people from a country than the population of the country! \n");
	}
}

static void UiFilterB(interface* I, int argc, char** argv) {
	char* pattern = malloc(sizeof(char) * 50);
	if (argc == 4) {
		strcpy(pattern, "");
	}
	else {
		strcpy(pattern, argv[4]);
	}
	vector* A = CtrlFilter(I->C, pattern, &NamePatternMatchingCmp, &CopyCountry);
	if (A->size == 0) {
		printf("No country matches the filter condition! \n");
	}
	else {
		for (int i = 0; i < VecGetSize(A); i++) {
			country* c = VecGetElement(A, i);
			printf("%s %s %d \n", GetName(c), GetContinent(c), GetPopulation(c));
		}
	}
	VecDestroy(&A, &CountryDestroy);
	free(pattern);
}

static void UiFilterBbonus(interface* I, int argc, char** argv) {
	char* pattern = malloc(sizeof(char) * 50);
	if (argc == 4) {
		strcpy(pattern, "");
	}
	else {
		strcpy(pattern, argv[4]);
	}
	vector* A = CtrlFilter(I->C, pattern, &ContinentPatternMatchingCmp, &CopyCountry);
	if (A->size == 0) {
		printf("No country matches the filter condition! \n");
	}
	else {
		VecSort(A, &AscendingByPopulation);
		for (int i = 0; i < VecGetSize(A); i++) {
			country* c = VecGetElement(A, i);
			printf("%s %s %d \n", GetName(c), GetContinent(c), GetPopulation(c));
		}
	}
	VecDestroy(&A, &CountryDestroy);
	free(pattern);
}

static void UiFilterLab3Activity(interface* I, int argc, char** argv) {
	vector* A;
	if (argc == 6) {
		A = CtrlFilter(I->C, argv[5], &FromContinent, &CopyCountry);
	}
	else {
		A = CtrlGetAll(I->C);
	}
	if (A->size == 0) {
		printf("No country matches the filter condition! \n");
	}
	else {
		if (strcmp(argv[1], "ascending") == 0) {
			VecSort(A, &AscendingByName);
		}
		else if (strcmp(argv[1], "descending") == 0) {
			VecSort(A, &DescendingByName);
		}
		else {
			printf("Incorect type of sorting! The application sorted implicitly ascending by name! \n");
			VecSort(A, &AscendingByName);
		}
		
		for (int i = 0; i < VecGetSize(A); i++) {
			country* c = VecGetElement(A, i);
			printf("%s %s %d \n", GetName(c), GetContinent(c), GetPopulation(c));
		}
	}
	VecDestroy(&A, &CountryDestroy);
}

static void UiFilterC(interface* I, int argc, char** argv) {
	vector* A;
	if (argc == 10) {
		A = CtrlFilter(I->C, argv[9], &FromContinent, &CopyCountry);
	}
	else {
		A = CtrlGetAll(I->C);
	}
	A = VecFilter(A, ToInt(argv[7]), &PopulationMoreThan, &CopyCountry);

	if (A->size == 0) {
		printf("No country matches the filter condition! \n");
	}
	else {
		if (strcmp(argv[1], "ascending") == 0) {
			VecSort(A, &AscendingByPopulation);
		}
		else if (strcmp(argv[1], "descending") == 0) {
			VecSort(A, &DescendingByPopulation);
		}
		else {
			printf("Incorect type of sorting! The application sorted implicitly ascending by population! \n");
			VecSort(A, &AscendingByPopulation);
		}

		for (int i = 0; i < VecGetSize(A); i++) {
			country* c = VecGetElement(A, i);
			printf("%s %s %d \n", GetName(c), GetContinent(c), GetPopulation(c));
		}
	}
	VecDestroy(&A, &CountryDestroy);
}

static void UiFilterLab4Activity(interface* I, int argc, char** argv) {
	vector* A;
	if (argc == 8) {
		A = CtrlFilter(I->C, argv[7], &NamePatternMatchingCmp, &CopyCountry);
	}
	else {
		A = CtrlFilter(I->C, "", &NamePatternMatchingCmp, &CopyCountry);
	}
	if (A->size == 0) {
		printf("No country matches the filter condition! \n");
		return;
	}
	VecSort(A, &DescendingByPopulation);
	for (int i = 0; i < VecGetSize(A); i++) {
		country* c = VecGetElement(A, i);
		printf("%s %s %d \n", GetName(c), GetContinent(c), GetPopulation(c));
	}
	VecDestroy(&A, &CountryDestroy);

}

static void UiFilter(interface* I, int argc, char** argv) {
	if ((argc == 5 || argc == 4)) {
		if (strcmp(argv[3], "with") == 0 && strcmp(argv[2], "name") == 0) {
			UiFilterB(I, argc, argv);
		}
		else if (strcmp(argv[3], "with") == 0 && strcmp(argv[2], "continent") == 0) {
			UiFilterBbonus(I, argc, argv);
		}
	}
	else if ((argc == 7 || argc == 8) && strcmp(argv[1], "descending") == 0 && strcmp(argv[3], "population") == 0) {
		UiFilterLab4Activity(I, argc, argv);
	}
	else if ((argc == 5 || argc == 6) && strcmp(argv[4], "from") == 0) {
		UiFilterLab3Activity(I, argc, argv);
	} 
	else if ((argc == 9 || argc == 10) && strcmp(argv[8], "from") == 0) {
		UiFilterC(I, argc, argv);
	}
	else {
		printf("Invalid filter! \n");
	}
}

static void UiUndo(interface* I, char* undoType) {
	if (strcmp(undoType, "normal") == 0) {
		if (CtrlUndo(I->C) != 0) {
			printf("Cannot undo right now! \n");
		}
		return;
	}
	if (CtrlUndoBonus(I->C) != 0) {
		printf("Cannot undo right now! \n");
	}
}

static void UiRedo(interface* I, char* undoType) {
	if (strcmp(undoType, "normal") == 0) {
		if (CtrlRedo(I->C) != 0) {
			printf("Cannot redo right now! \n");
		}
		return;
	}
	if (CtrlRedoBonus(I->C) != 0) {
		printf("Cannot redo right now! \n");
	}
}

void RunProgram() {

	UiPrintMenu();

	char *command = (char*)malloc(sizeof(char) * 1000);

	interface* I = CreateInterface();
	SetUp(I);

	char* undoType = (char*)malloc(sizeof(char) * 100);
	printf("What type of undo do you want? \nType normal or bonus: ");
	gets(undoType);

	if (strcmp(undoType, "normal") != 0 && strcmp(undoType, "bonus") != 0) {
		printf("Invalid option! The undo/redo will be implicitly done in the list of lists aproach. \n");
	}

	while (1) {
		printf("Give a command: ");
		gets(command);
		int argc = 0;
		char** argv = SplitCommand(command, &argc);
		if (strcmp(argv[0], "exit") == 0) {
			for (int i = 0; i < argc; i++) {
				free(argv[i]);
			}
			free(argv);
			break;
		}
		else if (strcmp(argv[0], "list") == 0) {
			UiPrintAll(I);
		} 
		else if (strcmp(argv[0], "add") == 0) {
			UiAdd(I, argc, argv);
		}
		else if (strcmp(argv[0], "delete") == 0) {
			UiDelete(I, argc, argv);
		}
		else if (strcmp(argv[0], "update") == 0) {
			UiUpdate(I, argc, argv);
		}
		else if (strcmp(argv[0], "migrate") == 0) {
			UiMigrate(I, argc, argv);
		}
		else if (strcmp(argv[0], "filter") == 0) {
			UiFilter(I, argc, argv);
		}
		else if (strcmp(argv[0], "undo") == 0) {
			UiUndo(I, undoType);
		}
		else if (strcmp(argv[0], "redo") == 0) {
			UiRedo(I, undoType);
		}
		else {
			printf("Ivalid command! Read again the instructions! \n");
			Instructions();
		}
		for (int i = 0; i < argc; i++) {
			free(argv[i]);
		}
		free(argv);
	}
	DestroyInterface(&I);
	free(command);
	free(undoType);
}