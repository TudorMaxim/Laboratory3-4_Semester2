#pragma once
#include "vector.h"
#include "stack.h"

typedef struct myRepository {
	vector* V;
} repository;

typedef struct myOperation {
	char* operationName;
	int number;
	country* c1;
	country* c2;
} operation;

repository* RepoCreate();
void RepoDestroy(repository** repo);

vector* RepoGetAll(repository* repo);

int RepoFind(repository* repo, char* name);
int RepoAdd(repository* repo, country* c);
int RepoDelete(repository* repo, char *name);
int RepoUpdate(repository* repo, char* name, country* c);
int RepoMigrate(repository* repo, int number, char* from, char* to);
vector* RepoFilter(repository* repo, genericType pattern, CompareFunctionType cmp, CopyFunctionType copy);

operation* OperationCreate(char* name, int number, country* c1, country* c2);
void OperationDestroy(operation** Op);
operation* OperationCopy(operation* op);