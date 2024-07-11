// main starting point of the application

#include <iostream>
#include "interface.h"

int main(int *argcp, char**argv)
{
	interface GUI;
	GUI.InitWindow(argcp, argv);
	return 0;
}