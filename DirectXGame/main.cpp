#include "AppWindow.h"
#include <wchar.h>
#include <stdlib.h>

// https://www.youtube.com/playlist?list=PLv8DnRaQOs5-ST_VDqgbbMRtzMtpK36Hy

int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
	try {
		GraphicsEngine::create();
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

	return 0;
}