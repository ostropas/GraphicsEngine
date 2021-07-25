#include "InputSystem.h"
#include <Windows.h>
#include <Xinput.h>

InputSystem* InputSystem::m_system = nullptr;

InputSystem::InputSystem()
{
	m_gamepad = new Gamepad();
}

InputSystem::~InputSystem()
{
	InputSystem::m_system = nullptr;
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

	for (unsigned int i = 0;i < 256; i++)
	{
		m_keys_state[i] = ::GetAsyncKeyState(i);
		// Key is down
		if (m_keys_state[i] & 0x8001) {
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

		// store current keys state to old state buffer
		::memcpy(m_old_keys_state, m_keys_state, sizeof(USHORT) * 256);
	}

	if (m_gamepad->Refresh()) {
		std::map<InputListner*, InputListner*>::iterator it = m_map_listners.begin();
		while (it != m_map_listners.end()) {

			if (abs(m_gamepad->leftStickX) > 0.1f || abs(m_gamepad->leftStickY) > 0.1f) {
				it->second->onLeftStickMove(Point(100 * m_gamepad->leftStickX, 100 * m_gamepad->leftStickY));
			}

			if (abs(m_gamepad->rightStickX) > 0.1f || abs(m_gamepad->rightStickY) > 0.1) {
				it->second->onRightStickMove(Point(100 * m_gamepad->rightStickX, 100 * m_gamepad->rightStickY));
			}

			//for(WORD i = 0;  i <= 0x8000; i*=2)
			//{
			//	if (m_gamepad->IsPressed(i)) {
			//		it->second->onGamepadKeyDown(i);
			//	}

			//	if (i == 0)
			//		i = 1;
			//}

			++it;
		}
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
	return m_system;
}

void InputSystem::create()
{
	if (InputSystem::m_system) throw std::exception("Input System already created");
	InputSystem::m_system = new InputSystem();
}

void InputSystem::release()
{
	if (!InputSystem::m_system) return;
	delete InputSystem::m_system;
}
