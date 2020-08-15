#include "AppWindow.h"
#include <wchar.h>
#include <stdlib.h>

int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
	AppWindow app;
	if (app.init()) {
		::ShowWindow(::GetConsoleWindow(), SW_HIDE);
		while (app.isRun())
		{
			app.broadcast();
		}
	}
	return 0;
}
