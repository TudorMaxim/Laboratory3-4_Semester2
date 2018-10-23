// Laboratory3-4.cpp : Defines the entry point for the console application.
//
#include "ui.h"
#include "tests.h"

int main() {
	TestCountry();
	TestUtilities();
	TestVector();
	TestRepository();
	TestStack();
	
	RunProgram();
	printf("Goodbye! ;) \n");
	_CrtDumpMemoryLeaks();
	return 0;
}