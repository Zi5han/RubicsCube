#pragma once

#include "KeyboardObserver.h"
#include <map>
#include <memory>
#include <glm/mat4x4.hpp>

class InputSystem {
public:
	enum ClickState {
		NO_ACTION = 0,
		CLICK = 1,
		HOLD = 2,
		RELEASE = 3
	};

	enum MouseButton {
		NO_BUTTON = -1,
		LEFT_BUTTON = 0,
		RIGHT_BUTTON = 1
	};

	//GENERAL
	void Initialize(GLFWwindow* window, const glm::mat4& viewProjection = glm::mat4(1.0f));
	void SetWindow(GLFWwindow* window) { m_window = window; }
	void Update();
	void ObserveKey(int key);
	void SetViewProjection(const glm::mat4& viewProjection) { m_viewProjection = viewProjection; }

	//MOUSE
	InputSystem::ClickState GetLeftClickState() const { return m_leftClickState; }
	InputSystem::ClickState GetRightClickState() const { return m_rightClickState; }

	glm::vec2 GetScreenPosition() const { return m_screenPosition; }
	glm::vec2 GetDragStartScreenPosition() const { return m_dragStartScreenPosition; }
	void GetPickingRay(glm::vec3& out_origin, glm::vec3& out_direction) const;
	void GetDragStartPickingRay(glm::vec3& out_origin, glm::vec3& out_direction) const;

	glm::ivec2 GetMouseWheelScrollOffset() const;

	//KEYBOARD
	bool IsKeyDown(int key) const { return m_keyMapper[key]->m_isDown; }
	bool WasKeyPressed(int key) const { return m_keyMapper[key]->m_wasPressed; }
	bool WasKeyReleased(int key) const { return m_keyMapper[key]->m_wasReleased; }
	MouseButton GetActiveMouseButton() const { return m_activeMouseButton; }

	//HELPING METHODS
	glm::vec2 NormalizeScreenVector(const glm::vec2& screenPosition) const;
	glm::vec2 WorldToScreen(const glm::vec3& worldPosition) const;

private:
	mutable std::map<int, std::unique_ptr<KeyboardObserver>> m_keyMapper;
	GLFWwindow* m_window;

	ClickState m_leftClickState;
	ClickState m_rightClickState;
	MouseButton m_activeMouseButton;

	glm::vec2 m_screenPosition;

	glm::vec2 m_dragStartScreenPosition;
	glm::vec3 m_dragStartRayOrigin;
	glm::vec3 m_dragStartRayDirection;

	glm::mat4 m_viewProjection;

	//HELPING METHODS
	void h_UpdateClickState(MouseButton mouseButton, ClickState& clickState);

	//STATIC
	static glm::ivec2 s_mouseScrollOffset;
	static void scrollCallback(GLFWwindow* window, double xScroll, double yScroll) {
		s_mouseScrollOffset.x = static_cast<int>(xScroll);
		s_mouseScrollOffset.y = static_cast<int>(yScroll);
	}
};