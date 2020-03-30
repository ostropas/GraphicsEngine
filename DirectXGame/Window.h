#pragma once
#include <Windows.h>

class Window
{
public:
	// Initialize the window
	bool init();
	bool broadcast();
	// Release the window
	bool release();
	bool isRun();

	// Events
	virtual void onCreate()=0;
	virtual void onUpdate()=0;
	virtual void onDestroy();
protected:
	HWND m_hwnd;
	bool m_is_run;
};

