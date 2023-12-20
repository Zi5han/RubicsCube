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

void InputSystem::SetViewProjection(const glm::mat4& projectionView) {
	m_projectionViewMat = projectionView;
}

//MOUSE
glm::vec2 InputSystem::GetMouseWheelScrollOffset() const {
	glm::ivec2 mouseScrollOffset = s_mouseScrollOffset;
	s_mouseScrollOffset = glm::vec2(0.0f, 0.0f);
	return mouseScrollOffset;
}

void InputSystem::GetPickingRay(glm::vec3& startingPoint, glm::vec3& direction) const {
	float xPosition, yPosition;
	GetMousePosition(xPosition, yPosition);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

	xPosition = (xPosition / screenWidth) * 2.0f - 1.0f;
	yPosition = 1.0f - (yPosition / screenHeight) * 2.0f;

	glm::vec4 nearPoint = glm::vec4(static_cast<float>(xPosition), static_cast<float>(yPosition), -0.99f, 1.0f);
	glm::vec4 farPoint = nearPoint;
	farPoint.z = 0.99f;

	glm::mat4 inverse = glm::inverse(m_projectionViewMat);
	nearPoint = inverse * nearPoint;
	farPoint = inverse * farPoint;

	nearPoint /= nearPoint.w;
	farPoint /= farPoint.w;

	startingPoint = nearPoint;
	direction = farPoint - nearPoint;

	direction = glm::normalize(direction);
}

void InputSystem::GetMousePosition(float& xPos, float& yPos) const {
	double xPos_double, yPos_double;
	glfwGetCursorPos(m_window, &xPos_double, &yPos_double);

	xPos = static_cast<float>(xPos_double);
	yPos = static_cast<float>(yPos_double);
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
		GetMousePosition(m_dragStartPosition.x, m_dragStartPosition.y);
		//GetMouseNormalizedPosition(m_mouseDragStartNormalizedScreenPosition.x, m_mouseDragStartNormalizedScreenPosition.y);

		GetMousePosition(m_dragStartPosition.x, m_dragStartPosition.y);
	}
	else {
		clickState = ClickState::HOLD;
	}
}

void InputSystem::NormalizeMousePosition(const float xAbsPos, const float yAbsPos, float& out_xNormPos, float& out_yNormPos) const {
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

	out_xNormPos = xAbsPos / screenWidth;
	out_yNormPos = yAbsPos / screenHeight;
}

//STATIC
glm::ivec2 InputSystem::s_mouseScrollOffset = glm::vec2(0.0f, 0.0f);