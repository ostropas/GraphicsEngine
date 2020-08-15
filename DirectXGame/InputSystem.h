#pragma once
#include "InputListner.h" 
#include <map>
#include "Point.h"

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void update();
	void addListner(InputListner* listener);
	void removeListner(InputListner* listener);

public:
	static InputSystem* get();
private:
	std::map<InputListner*, InputListner*> m_map_listners;
	unsigned char m_keys_state[256] = {};
	unsigned char m_old_keys_state[256] = {};
	Point m_old_mouse_pos;
	bool m_first_time = true;
};

