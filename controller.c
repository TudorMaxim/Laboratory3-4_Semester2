#include "controller.h"

//Function that creates an empty controller
controller* CtrlCreate() {
	controller* C = (controller*)malloc(sizeof(controller));
	C->repo = RepoCreate();
	C->undoStack = StackCreate(&VecCreate);
	C->redoStack = StackCreate(&VecCreate);
	C->undoStackOps = StackCreate(&OperationDestroy);
	C->redoStackOps = StackCreate(&OperationDestroy);
	return C;
}

//Function that destroys the controller
void CtrlDestroy(controller** C) {
	RepoDestroy(&((*C)->repo));
	StackDestroy(&((*C)->undoStack), &VecDestroy);
	StackDestroy(&((*C)->redoStack), &VecDestroy);
	StackDestroy(&((*C)->undoStackOps), &OperationDestroy);
	StackDestroy(&((*C)->redoStackOps), &OperationDestroy);
	free(*C);
	*C = NULL;
}

//Function that return all the elements from the controller
vector* CtrlGetAll(controller* C) {
	return RepoGetAll(C->repo);
}

/*
	Function that adds an element to controller.
	input:  C - the initial controller
			c - the country that is added
	output: C - the new controller with the c country added
*/
int CtrlAdd(controller* C, country* c) {

	vector* W = CtrlGetAll(C);
	vector* to_push = VecCopy(W, &CountryDestroy, &CopyCountry);
	Push(C->undoStack, to_push);

	operation* op = OperationCreate("add", 0, CopyCountry(c), CopyCountry(c));
	Push(C->undoStackOps, op);
	int exitcode = RepoAdd(C->repo, c);
	if (exitcode != 0) {
		Pop(C->undoStack);
	}
	return exitcode;
}

/*
	Function that deletes an element from controller.
	input:  C - the initial controller
			name - the name of the country which is deleted
	output: C - the new controller without the country with the given name
*/

int CtrlDelete(controller* C, char* name) {
	vector* W = CtrlGetAll(C);
	vector* to_push = VecCopy(W, &CountryDestroy, &CopyCountry);
	Push(C->undoStack, to_push);

	int pos = RepoFind(C->repo, name);
	if (pos == -1) {
		return 1;
	}
	country* c = VecGetElement(C->repo->V, pos);
	operation* op = OperationCreate("delete", 0, CopyCountry(c), CopyCountry(c));
	Push(C->undoStackOps, op);

	int exitcode = RepoDelete(C->repo, name);
	if (exitcode != 0) {
		Pop(C->undoStack);
	}
	return exitcode;
}

/*
	Function that updates a country existing in the controller
	input:  C - the initial controller
			name - the name of the country which is updated - string
			c - the new country - country*
	output: C - the controller with the given country updated
*/
int CtrlUpdate(controller* C, char* name, country* c) {
	vector* W = CtrlGetAll(C);
	vector* to_push = VecCopy(W, &CountryDestroy, &CopyCountry);
	Push(C->undoStack, to_push);

	int pos = RepoFind(C->repo, name);
	if (pos != -1) {
		country* old_c = VecGetElement(C->repo->V, pos);
		operation* op = OperationCreate("update", 0, CopyCountry(old_c), CopyCountry(c));
		Push(C->undoStackOps, op);
	}

	int exitcode =  RepoUpdate(C->repo, name, c);
	if (exitcode != 0) {
		Pop(C->undoStack);
	}
	return exitcode;
}

/*
	Function that updates the 2 countries in case of migration
	input:  C - the initial controller
			number - the number of people that migrate - integer
			from - the name of the country from which people migrate - string
			to - the name of the country to which people migrate - string
	output: C - the updated Controller
*/
int CtrlMigrate(controller* C, int number, char* from, char* to) {
	vector* W = CtrlGetAll(C);
	vector* to_push = VecCopy(W, &CountryDestroy, &CopyCountry);
	Push(C->undoStack, to_push);

	int pos_from = RepoFind(C->repo, from);
	int pos_to = RepoFind(C->repo, to);

	if (pos_from != -1 && pos_to != -1) {
		country* c1 = VecGetElement(C->repo->V, pos_from);
		country* c2 = VecGetElement(C->repo->V, pos_to);
		operation* op = OperationCreate("migrate", number, CopyCountry(c1), CopyCountry(c2));
		Push(C->undoStackOps, op);
	}
	
	int exitcode =  RepoMigrate(C->repo, number, from, to);
	if (exitcode != 0) {
		Pop(C->undoStack);
	}
	return exitcode;
}

/*
Function that filters the countries from the controller.
	input:  C - the controller
		    pattern - the argument after which whe filter - genericType
			cmp - the comapre function - CompareFunctionType
			copy - the copy function for the elements from the controller - CopyFunctionType
	output: RepoFilter - a filtered vector with elements of genericType
*/
vector* CtrlFilter(controller* C, genericType pattern, CompareFunctionType cmp, CopyFunctionType copy) {
	return RepoFilter(C->repo, pattern, cmp, copy);
}

int CtrlUndoBonus(controller* C) {
	if (IsEmpty(C->undoStack)) {
		return 1;
	}
	vector* top = GetTop(C->undoStack);
	Push(C->redoStack, C->repo->V);
	C->repo->V = top;
	Pop(C->undoStack);
	return 0;
}

int CtrlRedoBonus(controller* C) {
	if (IsEmpty(C->redoStack)) {
		return 1;
	}
	vector* top = GetTop(C->redoStack);
	Push(C->undoStack, C->repo->V);
	C->repo->V = top;
	Pop(C->redoStack);
	return 0;
}

int CtrlUndo(controller* C) {
	if (IsEmpty(C->undoStackOps)) {
		return 1;
	}
	operation* op = GetTop(C->undoStackOps);
	Push(C->redoStackOps, OperationCopy(op));

	if (strcmp(op->operationName, "add") == 0) {
		char* name = malloc(sizeof(char) * 100);
		strcpy(name, GetName(op->c1));
		RepoDelete(C->repo, name);
		free(name);
	}
	if (strcmp(op->operationName, "delete") == 0) {
		RepoAdd(C->repo, CopyCountry(op->c1));
	}
	if (strcmp(op->operationName, "update") == 0) {
		char* name = malloc(sizeof(char) * 100);
		strcpy(name, GetName(op->c2));
		RepoUpdate(C->repo, name, CopyCountry(op->c1));
		free(name);
	}
	if (strcmp(op->operationName, "migrate") == 0) {
		char* from = malloc(sizeof(char) * 100);
		char* to = malloc(sizeof(char) * 100);
		strcpy(from, GetName(op->c2));
		strcpy(to, GetName(op->c1));
		RepoMigrate(C->repo, op->number, from, to);
		free(from);
		free(to);
	}
	Pop(C->undoStackOps);
	return 0;
}

int CtrlRedo(controller* C) {
	if (IsEmpty(C->redoStackOps)) {
		return 1;
	}
	operation* op = GetTop(C->redoStackOps);
	Push(C->undoStackOps, OperationCopy(op));

	if (strcmp(op->operationName, "add") == 0) {
		RepoAdd(C->repo, CopyCountry(op->c1));
	}
	if (strcmp(op->operationName, "delete") == 0) {
		char* name = malloc(sizeof(char) * 100);
		strcpy(name, GetName(op->c1));
		RepoDelete(C->repo, name);
		free(name);
	}
	if (strcmp(op->operationName, "update") == 0) {
		char* name = malloc(sizeof(char) * 100);
		strcpy(name, GetName(op->c1));
		RepoUpdate(C->repo, name, CopyCountry(op->c2));
		free(name);
	}
	if (strcmp(op->operationName, "migrate") == 0) {
		char* from = malloc(sizeof(char) * 100);
		char* to = malloc(sizeof(char) * 100);
		strcpy(from, GetName(op->c1));
		strcpy(to, GetName(op->c2));
		RepoMigrate(C->repo, op->number, from, to);
		free(from);
		free(to);
	}
	Pop(C->redoStackOps);
	return 0;
}