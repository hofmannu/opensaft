// main starting point of the graphical user interface of the application

#include "Interface.h"
#include <backward/backward.hpp>

backward::SignalHandling sh{};

int main(int *argcp, char**argv)
{
	Interface GUI;
	GUI.InitWindow(argcp, argv);
	return 0;
}