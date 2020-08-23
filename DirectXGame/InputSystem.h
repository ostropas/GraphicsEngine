#pragma once
#include "InputListner.h" 
#include <map>
#include "Point.h"
#include "Gamepad.h"

class InputSystem
{
private:
	InputSystem();
	~InputSystem();

public:
	void update();
	void addListner(InputListner* listener);
	void removeListner(InputListner* listener);

	void setCursorPosition(const Point& pos);
	void showCursor(bool show);

public:
	static InputSystem* get();
	static void create();
	static void release();
private:
	Gamepad* m_gamepad;

	std::map<InputListner*, InputListner*> m_map_listners;
	unsigned char m_keys_state[256] = {};
	unsigned char m_old_keys_state[256] = {};
	Point m_old_mouse_pos;
	bool m_first_time = true;
	static InputSystem* m_system;
};

