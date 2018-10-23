#pragma once
#include "repository.h"

typedef struct myController {
	repository* repo;
	stack* undoStack;
	stack* redoStack;
	stack* undoStackOps;
	stack* redoStackOps;

} controller;

controller* CtrlCreate();
void CtrlDestroy(controller** C);

vector* CtrlGetAll(controller* C);
int CtrlAdd(controller* C, country* c);
int CtrlDelete(controller* C, char* name);
int CtrlUpdate(controller* C, char* name, country* new_c);
int CtrlMigrate(controller* C, int number, char* from, char* to);
vector* CtrlFilter(controller* C, genericType pattern, CompareFunctionType cmp, CopyFunctionType copy);
int CtrlUndoBonus(controller* C);
int CtrlRedoBonus(controller* C);
int CtrlUndo(controller* C);
int CtrlRedo(controller* C);