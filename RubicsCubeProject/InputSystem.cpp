#include "InputSystem.h"
#include <GLFW/glfw3.h>

void InputSystem::Update() {
	for (auto i = m_keyMapper.begin(); i != m_keyMapper.end(); i++)
		i->second->Update();
}

void InputSystem::ObserverKey(int key) {
	m_keyMapper[key] = std::make_unique<KeyboardObserver>(KeyboardObserver(m_window, key));
}

bool InputSystem::IsLeftMouseButtonDown()
{
	return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

bool InputSystem::IsRightMouseButtonDown()
{
	return glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
}
