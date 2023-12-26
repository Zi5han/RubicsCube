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

	void SetWindow(GLFWwindow* window) { m_window = window; };
	void FetchInputs();
	void ObserveKey(int key);
	void SetViewProjection(const glm::mat4& projectionView);

	glm::vec2 GetMouseWheelScrollOffset() const;
	InputSystem::ClickState GetLeftClickState() const { return m_leftClickState; };
	InputSystem::ClickState GetRightClickState() const { return m_rightClickState; };

	void GetPickingRay(glm::vec3& out_origin, glm::vec3& out_direction) const;
	glm::vec2 GetMouse2DPosition(bool normalize = false) const;

	void GetDragStartPickingRay(glm::vec3& out_origin, glm::vec3& out_direction) const;
	glm::vec2 GetDragStart2DMousePosition(bool normalize = false) const;

	bool IsKeyDown(int key) const { return m_keyMapper[key]->m_isDown; };
	bool WasKeyPressed(int key) const { return m_keyMapper[key]->m_wasPressed; };
	bool WasKeyReleased(int key) const { return m_keyMapper[key]->m_wasReleased; };

private:
	mutable std::map<int, std::unique_ptr<KeyboardObserver>> m_keyMapper;
	GLFWwindow* m_window;

	ClickState m_leftClickState;
	ClickState m_rightClickState;

	glm::vec2 m_dragStartScreenPosition;
	glm::vec3 m_dragStartRayOrigin;
	glm::vec3 m_dragStartRayDirection;

	glm::mat4 m_projectionViewMat;

	//HELPING METHODS
	void UpdateClickState(int button, ClickState& clickState);

	void NormalizeMousePosition(glm::vec2& out_normPos) const;

	//STATIC
	static glm::ivec2 s_mouseScrollOffset;
	static void scrollCallback(GLFWwindow* window, double xScroll, double yScroll) {
		s_mouseScrollOffset.x = static_cast<int>(xScroll);
		s_mouseScrollOffset.y = static_cast<int>(yScroll);
	};
};