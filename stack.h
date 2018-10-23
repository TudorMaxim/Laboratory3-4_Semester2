#pragma once

#include "vector.h"

typedef struct myStack {
	vector* V;
} stack;

stack* StackCreate(DestroyFunctionType f);
void StackDestroy(stack** S, DestroyFunctionType f);

genericType GetTop(stack* S);
void Push(stack* S, genericType e);
void Pop(stack* S);
int IsEmpty(stack* S);