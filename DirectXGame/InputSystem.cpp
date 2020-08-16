#include "InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if (m_first_time) {
		m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);
		m_first_time = false;
	}

	if (current_mouse_pos.x != m_old_mouse_pos.m_x || current_mouse_pos.y != m_old_mouse_pos.m_y) {
		// there is mouse move event

		std::map<InputListner*, InputListner*>::iterator it = m_map_listners.begin();

		while (it != m_map_listners.end()) {
			it->second->onMouseMove(
				Point(current_mouse_pos.x, current_mouse_pos.y));
			++it;
		}
	}
	m_old_mouse_pos = Point(current_mouse_pos.x, current_mouse_pos.y);

	if (::GetKeyboardState(m_keys_state)) {
		for (unsigned int i = 0;i < 256; i++)
		{
			// Key is down
			if (m_keys_state[i] & 0x80) {
				std::map<InputListner*, InputListner*>::iterator it = m_map_listners.begin();

				while (it != m_map_listners.end()) {
					if (i == VK_LBUTTON) {
						if (m_keys_state[i] != m_old_keys_state[i]) {
							it->second->onLeftMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
					}
					else if (i == VK_RBUTTON) {
						if (m_keys_state[i] != m_old_keys_state[i]) {
							it->second->onRightMouseDown(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
					}
					else
						it->second->onKeyDown(i);
					

					++it;
				}
			}
			else // Key is up
			{
				if (m_keys_state[i] != m_old_keys_state[i]) {
					std::map<InputListner*, InputListner*>::iterator it = m_map_listners.begin();

					while (it != m_map_listners.end()) {
						if (i == VK_LBUTTON) {
							it->second->onLeftMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else if (i == VK_RBUTTON) {
							it->second->onRightMouseUp(Point(current_mouse_pos.x, current_mouse_pos.y));
						}
						else
							it->second->onKeyUp(i);

						++it;
					}
				}
			}
		}

		// store current keys state to old state buffer
		::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListner(InputListner* listener)
{
	m_map_listners.insert(std::make_pair<InputListner*, InputListner*>
		(std::forward<InputListner*>(listener), std::forward<InputListner*>(listener)));
}

void InputSystem::removeListner(InputListner* listener)
{
	std::map<InputListner*, InputListner*>::iterator it = m_map_listners.find(listener);

	if (it != m_map_listners.end()) {
		m_map_listners.erase(it);
	}
}

void InputSystem::setCursorPosition(const Point& pos)
{
	::SetCursorPos(pos.m_x, pos.m_y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}
