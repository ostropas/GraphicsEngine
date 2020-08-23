#include "AppWindow.h"
#include <wchar.h>
#include <stdlib.h>
#include "InputSystem.h"

// https://www.youtube.com/playlist?list=PLv8DnRaQOs5-ST_VDqgbbMRtzMtpK36Hy

int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
	try {
		GraphicsEngine::create();
		InputSystem::create();
	}
	catch (...) { return -1; }

	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	try {
		AppWindow app;
		while (app.isRun());
	}
	catch (...) {
		GraphicsEngine::release();
		InputSystem::release();

		return -1;
	}

	GraphicsEngine::release();
	InputSystem::release();

	return 0;
}