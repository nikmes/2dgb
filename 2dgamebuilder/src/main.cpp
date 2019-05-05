/// <summary>
/// Entry Point of the application
/// </summary>

#include "editstate.h"
#include "introstate.h"
#include "sandboxstate.h"
#include "applicationstate.h"
#include "application.h"

#ifdef _WIN32 || _WIN64
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup" )
#endif

/// <summary>
/// 
/// </summary>
/// <returns></returns>
/// 
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
