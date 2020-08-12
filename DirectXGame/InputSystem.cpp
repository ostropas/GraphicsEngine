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
	if (::GetKeyboardState(m_keys_state)) {
		for (unsigned int i = 0;i < 256; i++)
		{
			// Key is down
			if (m_keys_state[i] & 0x80) {
				std::map<InputListner*, InputListner*>::iterator it = m_map_listners.begin();

				while (it != m_map_listners.end()) {
					it->second->onKeyDown(i);
					++it;
				}
			}
			else // Key is up
			{
				if (m_keys_state[i] != m_old_keys_state[i]) {
					std::map<InputListner*, InputListner*>::iterator it = m_map_listners.begin();

					while (it != m_map_listners.end()) {
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

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}
