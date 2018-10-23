#include "stack.h"

/*
	Function that creates a new stack
		input: 
			f - pointer to the destroy function associated with the type of the elements
		output: 
			S - a dynamic allocated stack with generic elements
*/

stack* StackCreate(DestroyFunctionType f) {
	int capacity = 8;
	stack* S = (stack*)malloc(sizeof(stack));
	S->V = VecCreate(capacity, f);
	return S;
}

/*
	Function that destroys a vector.
		input: S - pointer to a dynamic stack
			   f -  pointer to the destroy function associated with the type of the elements
		output: -
*/
void StackDestroy(stack** S, DestroyFunctionType f) {
	VecDestroy(&((*S)->V), f);
	free(*S);
	*S = NULL;
}

/*
	Function that gets the top of the stack
	input: S - the stack
	output: GetTop - the top of the stack
*/
genericType GetTop(stack* S) {
	int len = VecGetSize(S->V);
	if (len > 0) 
		return VecGetElement(S->V, len - 1);
	return -1;
}

/*
	Function that pushes an element to a stack.
		input: S - the initial stack
			   e - the element that is added
		output: V - the new stack with the e element pushed
*/
void Push(stack* S, genericType e) {
	VecAdd(S->V, e);
}

/*
	Function that pop the stack
	input: S - the intial stack
	output: S - the stack after the pop operation
*/
void Pop(stack* S) {
	int len = VecGetSize(S->V);
	if (len > 0) VecDelete(S->V, len - 1);
}

// Check if the stack is empty
int IsEmpty(stack* S) {
	return S->V->size == 0 ? 1 : 0;
}