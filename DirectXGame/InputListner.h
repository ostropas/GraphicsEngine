#pragma once
#include "Point.h"
#include <Windows.h>

class InputListner {
public:
	InputListner() {

	}

	~InputListner() {

	}

	//Keyboard pure virtual callback function
	virtual void onKeyDown(int key) = 0;
	virtual void onKeyUp(int key) = 0;

	//Mouse pure virtual callback function
	virtual void onMouseMove(const Point& mouse_pos) = 0;

	virtual void onLeftMouseDown(const Point& mouse_pos) = 0;
	virtual void onLeftMouseUp(const Point& mouse_pos) = 0;
	virtual void onRightMouseDown(const Point& mouse_pos) = 0;
	virtual void onRightMouseUp(const Point& mouse_pos) = 0;

	//Gamepad pure virtual callback function
	virtual void onGamepadKeyDown(WORD key) = 0;
	virtual void onLeftStickMove(const Point& stick_pos) = 0;
	virtual void onRightStickMove(const Point& stick_pos) = 0;
};
