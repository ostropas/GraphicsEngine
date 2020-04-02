#include "AppWindow.h"

// https://www.youtube.com/playlist?list=PLv8DnRaQOs5-ST_VDqgbbMRtzMtpK36Hy

int main() {
	AppWindow app;
	if (app.init()) {

		while (app.isRun())
		{
			app.broadcast();
		}
	}
	return 0;
}