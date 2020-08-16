#include "AppWindow.h"
#include <wchar.h>
#include <stdlib.h>

// https://www.youtube.com/playlist?list=PLv8DnRaQOs5-ST_VDqgbbMRtzMtpK36Hy

int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
	AppWindow app;
	if (app.init()) {
		::ShowWindow(::GetConsoleWindow(), SW_SHOW);
		while (app.isRun())
		{
			app.broadcast();
		}
	}
	return 0;
}