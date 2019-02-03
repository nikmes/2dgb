/**
* \date 14/02/2015
* \author Nicholas Messaritis
*
* \file main.cpp
*
*/

#include "editstate.h"
#include "introstate.h"
#include "sandboxstate.h"
#include "applicationstate.h"
#include "application.h"

#ifdef _WIN32 || _WIN64
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup" )
#endif

int main()
{
	Application app;
	
	app.init();

	app.changeState(CEditState::Instance());

	while (app.running())
	{
		app.handleEvents();

		app.update(0);

		app.draw();
	}

	return 0;
}
