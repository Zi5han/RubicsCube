#include "InputSystem.h"
#include <GLFW/glfw3.h>

//GENERAL
void InputSystem::FetchInputs() {
	for (auto i = m_keyMapper.begin(); i != m_keyMapper.end(); i++)
		i->second->Update();
	UpdateClickState(GLFW_MOUSE_BUTTON_LEFT, m_leftClickState);
	UpdateClickState(GLFW_MOUSE_BUTTON_RIGHT, m_rightClickState);
}

void InputSystem::ObserveKey(int key) {
	m_keyMapper[key] = std::make_unique<KeyboardObserver>(KeyboardObserver(m_window, key));
}

void InputSystem::SetViewProjection(const glm::mat4& projectionViewMat) {
	m_projectionViewMat = projectionViewMat;
}

//MOUSE
glm::vec2 InputSystem::GetMouseWheelScrollOffset() const {
	glm::ivec2 mouseScrollOffset = s_mouseScrollOffset;
	s_mouseScrollOffset = glm::vec2(0.0f, 0.0f);
	return mouseScrollOffset;
}

void InputSystem::GetPickingRay(glm::vec3& out_origin, glm::vec3& out_direction) const {
	glm::vec2 position = GetMouse2DPosition();

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

	position.x = (position.x / screenWidth) * 2.0f - 1.0f;
	position.y = 1.0f - (position.y / screenHeight) * 2.0f;

	glm::vec4 nearPoint = glm::vec4(position.x, position.y, -0.99f, 1.0f);
	glm::vec4 farPoint = nearPoint;
	farPoint.z = 0.99f;

	glm::mat4 inverse = glm::inverse(m_projectionViewMat);
	nearPoint = inverse * nearPoint;
	farPoint = inverse * farPoint;

	nearPoint /= nearPoint.w;
	farPoint /= farPoint.w;

	out_origin = nearPoint;

	out_direction = farPoint - nearPoint;
	out_direction = glm::normalize(out_direction);
}

glm::vec2 InputSystem::GetMouse2DPosition(bool normalize) const {
	glm::vec2 position;
	double xPos_double, yPos_double;
	glfwGetCursorPos(m_window, &xPos_double, &yPos_double);

	position.x = static_cast<float>(xPos_double);
	position.y = static_cast<float>(yPos_double);

	if (normalize)
		NormalizeMousePosition(position);
	return position;
}

void InputSystem::GetDragStartPickingRay(glm::vec3& out_origin, glm::vec3& out_direction) const {
		out_origin = glm::vec3(m_dragStartRayOrigin);
		out_direction = glm::vec3(m_dragStartRayDirection);
}

glm::vec2 InputSystem::GetDragStart2DMousePosition(bool normalize) const {
	glm::vec2 position = glm::vec2(m_dragStartScreenPosition);
	if (normalize)
		NormalizeMousePosition(position);
	return position;
}

//HELPING METHODS
void InputSystem::UpdateClickState(int button, ClickState& clickState) {
	if (!(glfwGetMouseButton(m_window, button) == GLFW_PRESS)) {
		if (clickState == ClickState::CLICK || clickState == ClickState::HOLD) {
			clickState = ClickState::RELEASE;
		}
		else {
			clickState = ClickState::NO_ACTION;
		}
	}
	else if (clickState == ClickState::NO_ACTION || clickState == ClickState::RELEASE) {
		clickState = ClickState::CLICK;

		GetPickingRay(m_dragStartRayOrigin, m_dragStartRayDirection);
		m_dragStartScreenPosition = GetMouse2DPosition();
		//GetMouseNormalizedPosition(m_mouseDragStartNormalizedScreenPosition.x, m_mouseDragStartNormalizedScreenPosition.y);

		m_dragStartScreenPosition = GetMouse2DPosition();
	}
	else {
		clickState = ClickState::HOLD;
	}
}

void InputSystem::NormalizeMousePosition(glm::vec2& out_normPos) const {
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

	out_normPos.x = out_normPos.x / screenWidth;
	out_normPos.y = out_normPos.y / screenHeight;
}

//STATIC
glm::ivec2 InputSystem::s_mouseScrollOffset = glm::vec2(0.0f, 0.0f);