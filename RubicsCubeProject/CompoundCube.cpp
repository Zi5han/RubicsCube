#include "CompoundCube.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>

void CompoundCube::Initialize(GLFWwindow* window) {
	m_input.SetWindow(window);
	m_input.ObserverKey(GLFW_KEY_SPACE);
	m_input.ObserverKey(GLFW_KEY_RIGHT);
	m_input.ObserverKey(GLFW_KEY_LEFT);
	m_input.ObserverKey(GLFW_KEY_UP);
	m_input.ObserverKey(GLFW_KEY_DOWN);

	m_cubieRenderer.Initialize();
	m_turningAngle = 0.0f;
	m_orientationQuaternion = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
	m_actiualPosition = glm::vec3(0.0f);

	m_wasRightMouseButtonPressed = false;
}

void CompoundCube::Render(float aspectRatio) {

	float radius = 18.0f;
	m_actiualPosition.y = m_actiualPosition.y > 1 ? 1 : m_actiualPosition.y;
	m_actiualPosition.y = m_actiualPosition.y < -1 ? -1 : m_actiualPosition.y;
	float cameraHeight = sinf(m_actiualPosition.y);

	float horizontalDistance = cosf(m_actiualPosition.y);

	glm::vec3 cameraPosition = glm::vec3(
		horizontalDistance * cosf(m_actiualPosition.x),
		cameraHeight,
		horizontalDistance * sinf(m_actiualPosition.x)
	);

	cameraPosition *= radius;

	glm::mat4 m_viewProject = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f) *
		glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 finalMat = glm::translate(m_viewProject, glm::vec3(0.0f));

	m_cubieRenderer.Render(finalMat);

	//Offset + 0.1f damit die Luecken zwischen den Minicubies erscheinen.
	float offset = m_cubieRenderer.GetCubieExtention();
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				glm::mat4 compound = glm::translate(finalMat, glm::vec3((i - 1) * offset, (j - 1) * offset, (k - 1) * offset));
				m_cubieRenderer.Render(compound);
			}
		}
	}
}

void CompoundCube::ClearResources() {
	m_cubieRenderer.ClearResources();
}

void CompoundCube::Update(double deltaTime) {
	m_input.Update();

	if (!m_input.IsLeftMouseButtonDown()) {
		m_wasRightMouseButtonPressed = false;
		return;
	}
	if (!m_wasRightMouseButtonPressed) {
		m_wasRightMouseButtonPressed = true;
		m_input.GetMouseNormalizedPosition(m_oldPosition.x, m_oldPosition.y);
		return;
	}
	double currentXPosition, currentYPosition;

	m_input.GetMouseNormalizedPosition(currentXPosition, currentYPosition);

	const float sesitivity = 8;

	float xVelocity = (currentXPosition - m_oldPosition.x) * sesitivity;

	float yVelocity = (currentYPosition - m_oldPosition.y) * sesitivity;

	m_actiualPosition.x += xVelocity;
	m_actiualPosition.y += yVelocity;

	m_oldPosition.x = currentXPosition;
	m_oldPosition.y = currentYPosition;
}