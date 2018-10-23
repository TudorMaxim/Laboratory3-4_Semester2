#pragma once
#include "utilities.h"

typedef void* genericType;
typedef void (*DestroyFunctionType) (genericType* );
typedef genericType (*CopyFunctionType) (genericType);
typedef int(*CompareFunctionType) (genericType, genericType); // Returns 1 if ok, otherwise 0

typedef struct myVector {
	int size;
	int capacity;
	genericType* elems;
	DestroyFunctionType destroyFunction;
}vector;

/*
Function that creates a new vector
input: 
	capacity - an integer representing the initial capacity of the vector
	f - pointer to the destroy function associated with the type of the elements
output:
	V - a dynamic allocated vector with generic elements
*/
vector* VecCreate(int capacity, DestroyFunctionType f);

/*
Function that destroys a vector.
input: 
	V - pointer to a dynamic vector
	f -  pointer to the destroy function associated with the type of the elements
output: -
*/
void VecDestroy(vector** V, DestroyFunctionType f);

// Function that gets the size of the vector
int VecGetSize(vector* V);

// Function that gets an element on a given position from the vector
genericType VecGetElement(vector* V, int pos);

/*
Function that adds an element to a vector.
input:
	V - the initial vector
	t - the element that is added
output: 
	V - the new vector with the t element added
*/
void VecAdd(vector* V, genericType t);

/*
Function that deletes an element to a vector.
input:
	V - the initial vector
	t - the element that is deleted
output: 
	V - the new vector without the element t
*/
void VecDelete(vector* V, int pos);

/*
Function that updates an element from a vector.
input: 
	V - the initial vector
	pos - the position that is updated
	t - the new element
output: 
	V - the new vector element on the given position updated
*/
void VecUpdate(vector* V, int pos, genericType t);

/*
Function tha filter a vector.
input:
	V - the vector that is filtered
	x - an element with which the elements of the vector are compared
	cmp - a comparison function
	copy - a copy function for the elements of the vector
output: 
	VecFilter - return a filtered vector
*/
vector* VecFilter(vector* V, genericType x, CompareFunctionType cmp, CopyFunctionType copy);

/*
Function that copies a vector.
input:
	V - the vector which will be copied
	f - the destroy function specific to the elements of the vector
	copy - the copy function specific to the elements of the vector
output:
	VecCopy - return a coy of the vector V
*/
vector* VecCopy(vector* V, DestroyFunctionType f, CopyFunctionType copy);

/*
Function that sorts a vector.
input:	
	V - the vector which will be sorted.
	cmp - a comparison function for the sorting
output:
	V - the sorted vector
*/
void VecSort(vector* V, CompareFunctionType cmp);
