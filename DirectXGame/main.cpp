#include "AppWindow.h"
#include <wchar.h>
#include <stdlib.h>
#include "InputSystem.h"

int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
	try {
		GraphicsEngine::create();
		InputSystem::create();
	}
	catch (...) { return -1; }

	AppWindow app;
	if (app.init()) {
		::ShowWindow(::GetConsoleWindow(), SW_HIDE);
		while (app.isRun())
		{
			app.broadcast();
		}
	}

	GraphicsEngine::release();
	InputSystem::release();

	return 0;
}
