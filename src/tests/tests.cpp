//============================================================================
// Name        : tests.cpp
// Author      : Eclipse CDT
// Version     :
// Copyright   : Anyone can use this
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

extern int utilTestMain(int argc, char **argv);
extern int tableLookupTestMain(int argc, char **argv);

typedef int (*testType)(int, char**);
 testType rgTests[]={
		&utilTestMain,
		&tableLookupTestMain
};
unsigned int nTests = sizeof(rgTests)/sizeof(testType);

int main(int argc, char **argv) {
	cout << "!!!Hello World!!! " << nTests << endl; // prints !!!Hello World!!!

	for (unsigned int i = 0; i < nTests; i++)
	{
		testType test = rgTests[i];
		test(argc, argv);
	}
	return utilTestMain(0,0);
}
