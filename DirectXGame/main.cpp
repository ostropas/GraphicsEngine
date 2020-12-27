#include "SpaceShooterGame.h"
#include "InputSystem.h"

int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
	try {
		GraphicsEngine::create();
		InputSystem::create();
	}
	catch (...) { return -1; }

	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	try {
		SpaceShooterGame app;
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
