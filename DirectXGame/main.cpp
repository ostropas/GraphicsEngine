#include "AppWindow.h"

//http://www.youtube.com/watch?v=ssGka-bSTvQ

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