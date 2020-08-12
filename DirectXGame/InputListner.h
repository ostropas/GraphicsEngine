#pragma once

class InputListner {
public:
	InputListner() {

	}

	~InputListner() {

	}

	//Keyboard pure virtual callback function
	virtual void onKeyDown(int key) = 0;
	virtual void onKeyUp(int key) = 0;
};
