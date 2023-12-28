#include "InputSystem.h"
#include <GLFW/glfw3.h>

//GENERAL
void InputSystem::Initialize(GLFWwindow* window, const glm::mat4& viewProjection) {
	SetWindow(window);
	SetViewProjection(viewProjection);
	glfwSetScrollCallback(window, scrollCallback);
}

void InputSystem::Update() {
	// Update key inputs
	for (auto i = m_keyMapper.begin(); i != m_keyMapper.end(); i++)
		i->second->Update();

	// Update mouse button states
	UpdateClickState(GLFW_MOUSE_BUTTON_LEFT, m_leftClickState);
	UpdateClickState(GLFW_MOUSE_BUTTON_RIGHT, m_rightClickState);

	// Update current screen position
	double x, y;
	glfwGetCursorPos(m_window, &x, &y);

	m_screenPosition.x = static_cast<float>(x);
	m_screenPosition.y = static_cast<float>(y);
}

void InputSystem::ObserveKey(int key) {
	m_keyMapper[key] = std::make_unique<KeyboardObserver>(KeyboardObserver(m_window, key));
}

//MOUSE
void InputSystem::GetPickingRay(glm::vec3& out_origin, glm::vec3& out_direction) const {
	glm::vec2 position = NormalizeScreenVector(m_screenPosition);

	position.x = (position.x) * 2.0f - 1.0f;
	position.y = 1.0f - (position.y) * 2.0f;

	glm::vec4 nearPoint = glm::vec4(position.x, position.y, -0.99f, 1.0f);
	glm::vec4 farPoint = nearPoint;
	farPoint.z = 0.99f;

	glm::mat4 inverse = glm::inverse(m_viewProjection);
	nearPoint = inverse * nearPoint;
	farPoint = inverse * farPoint;

	nearPoint /= nearPoint.w;
	farPoint /= farPoint.w;

	out_origin = nearPoint;

	out_direction = farPoint - nearPoint;
	out_direction = glm::normalize(out_direction);
}

void InputSystem::GetDragStartPickingRay(glm::vec3& out_origin, glm::vec3& out_direction) const {
	out_origin = glm::vec3(m_dragStartRayOrigin);
	out_direction = glm::vec3(m_dragStartRayDirection);
}

glm::vec2 InputSystem::NormalizeScreenVector(const glm::vec2& screenPosition) const {
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(m_window, &screenWidth, &screenHeight);

	glm::vec2 normPos;

	normPos.x = screenPosition.x / screenWidth;
	normPos.y = screenPosition.y / screenHeight;

	return normPos;
}

glm::vec2 InputSystem::WorldToScreen(const glm::vec3& worldPosition) const {
	// Erstellen Sie eine 4D-Homogenisierungsmatrix für den gegebenen 3D-Weltvektor
	glm::vec4 h_WorldPosition = glm::vec4(worldPosition, 1.0f);

	// Multiplizieren Sie die Homogenisierungsmatrix mit der Projektion und View-Matrix
	glm::vec4 clipSpacePosition = m_viewProjection * h_WorldPosition;

	// Konvertieren Sie die Clip-Space-Position in Screen-Space
	glm::vec2 screenPosition;
	screenPosition.x = (clipSpacePosition.x / clipSpacePosition.w + 1.0f) / 2.0f;
	screenPosition.y = (1.0f - (clipSpacePosition.y / clipSpacePosition.w + 1.0f) / 2.0f);

	return screenPosition;
}

glm::ivec2 InputSystem::GetMouseWheelScrollOffset() const {
	glm::ivec2 mouseScrollOffset = s_mouseScrollOffset;
	s_mouseScrollOffset = glm::ivec2(0, 0);
	return mouseScrollOffset;
}

//HELPING METHODS
void InputSystem::UpdateClickState(int mouseButton, ClickState& clickState) {
	if (!(glfwGetMouseButton(m_window, mouseButton) == GLFW_PRESS)) {
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
		m_dragStartScreenPosition = GetScreenPosition();
		//GetMouseNormalizedPosition(m_mouseDragStartNormalizedScreenPosition.x, m_mouseDragStartNormalizedScreenPosition.y);

		m_dragStartScreenPosition = GetScreenPosition();
	}
	else {
		clickState = ClickState::HOLD;
	}
}

//STATIC
glm::ivec2 InputSystem::s_mouseScrollOffset = glm::vec2(0.0f, 0.0f);