#include <iostream>

#include "Root.h"

int main(int argc, char* argv[])
{

	//initiate root
	Root* mRoot;
	mRoot = new Root();

	mRoot->initEngine();


	delete mRoot;

	//initiate resource manager
	return 0;
}