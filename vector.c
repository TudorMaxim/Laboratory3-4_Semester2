#include "vector.h"

vector* VecCreate(int capacity, DestroyFunctionType f) {
	vector* V = (vector*) malloc(sizeof(vector));
	if (V == NULL) {
		return NULL; // EXIT_FAILURE
	}
	V->size = 0;
	V->capacity = capacity;
	V->elems = (genericType*)malloc(sizeof(genericType) * capacity);
	if (V->elems == NULL) {
		return NULL; // EXIT_FAILURE
	}
	V->destroyFunction = f;
	return V; //EXIT_SUCCESS;
}

void VecDestroy(vector** V, DestroyFunctionType f) {
	if (*V == NULL) {
		return;
	}
	for (int i = 0; i < (*V)->size; i++) {
		(*V)->destroyFunction(&((*V)->elems[i]));
	}
	free((*V)->elems);
	(*V)->elems = NULL;
	free (*V);
	*V = NULL;
}

int VecGetSize(vector* V) {
	return V->size;
}

genericType VecGetElement(vector* V, int pos) {
	return V->elems[pos];
}

void VecAdd(vector* V, genericType t) {
	if (V->capacity == V->size) {
		V->capacity <<= 1;
		V->elems = (genericType*)realloc(V->elems, sizeof(genericType) * V->capacity);
	}
	V->elems[V->size++] = t;
}

void VecDelete(vector* V, int pos) {
	if (pos < 0 || pos >= V->size)
		return;

	if (V->size <= (V->capacity >> 2)) {
		V->capacity >>= 1;
		V->elems = (genericType*)realloc(V->elems, sizeof(genericType) * V->capacity);
	}
	V->destroyFunction(&(V->elems[pos]));
	for (int i = pos; i < VecGetSize(V) - 1; i++) {
		V->elems[i] = V->elems[i + 1];
	}
	V->size--;
}

void VecUpdate(vector* V, int pos, genericType t) {
	V->destroyFunction(&(V->elems[pos]));
	V->elems[pos] = t;
}

vector* VecFilter(vector* V, genericType y, CompareFunctionType cmp, CopyFunctionType copy) {
	vector* W = VecCreate(8, V->destroyFunction);
	for (int i = 0; i < VecGetSize(V); i++) {
		genericType x = VecGetElement(V, i);
		if (cmp(x, y) == 1) {
			genericType toAdd = copy(x);
			VecAdd(W, toAdd);
		}
	}
	return W;
}

vector* VecCopy(vector* V, DestroyFunctionType f, CopyFunctionType copy) {
	vector* W = VecCreate(8, f);
	for (int i = 0; i < VecGetSize(V); i++) {
		genericType x = VecGetElement(V, i);
		genericType toAdd = copy(x);
		VecAdd(W, toAdd);
	}
	return W;
}


static void Merge(vector* V, int left, int mid, int right, CompareFunctionType cmp) {
	vector* A = VecCreate(V->capacity, V->destroyFunction);
	vector* B = VecCreate(V->capacity, V->destroyFunction);
	for (int i = left; i <= mid; i++) {
		VecAdd(A, VecGetElement(V, i));
	}
	for (int i = mid + 1; i <= right; i++) {
		VecAdd(B, VecGetElement(V, i));
	}
	int pos = left;
	int i = 0, j = 0;
	while (i < A->size && j < B->size) {
		if (cmp(VecGetElement(A, i), VecGetElement(B, j)) == 1) {
			V->elems[pos] = VecGetElement(A, i);
			i++;
			pos++;
		}
		else {
			V->elems[pos] = VecGetElement(B, j);
			j++;
			pos++;
		}
	}
	while (i < A->size) {
		V->elems[pos] = VecGetElement(A, i);
		i++;
		pos++;
	}
	while (j < B->size) {
		V->elems[pos] = VecGetElement(B, j);
		j++;
		pos++;
	}
}

static void MergeSort(vector* V, int left, int right, CompareFunctionType cmp) {
	if (left >= right) return;
	int mid = (left + right) >> 1;
	MergeSort(V, left, mid, cmp);
	MergeSort(V, mid + 1, right, cmp);
	Merge(V, left, mid, right, cmp);
}

void VecSort(vector* V, CompareFunctionType cmp) {
	MergeSort(V, 0, V->size - 1, cmp);
}


