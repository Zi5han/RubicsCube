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

	m_wasRightMouseButtonPressed = false;
}

void CompoundCube::Render(float aspectRatio) {
	glm::mat4 globalTransformation = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f) *
		glm::lookAt(glm::vec3(0.0f, 0.0f, -18.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::mat4_cast(m_orientationQuaternion);

	//Offset + 0.1f damit die Luecken zwischen den Minicubies erscheinen.
	float offset = m_cubieRenderer.GetCubieExtention();
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				glm::mat4 compound = glm::translate(globalTransformation, glm::vec3((i - 1) * offset, (j - 1) * offset, (k - 1) * offset));

				//Rotation der mittleren horizontalen auf jeder Seite
				//compound = glm::rotate(compound, glm::radians(90.0f) * (i % 2), glm::vec3(1.0f, 0.0f, 0.0f));
				//compound = glm::rotate(compound, glm::radians(90.0f) * (j % 2), glm::vec3(0.0f, 1.0f, 0.0f));
				//compound = glm::rotate(compound, glm::radians(90.0f) * (k % 2), glm::vec3(0.0f, 0.0f, 1.0f));
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
	if (m_input.IsKeyDown(GLFW_KEY_SPACE))
		m_orientationQuaternion = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

	if (!m_input.IsRightMouseButtonDown()) {
		m_wasRightMouseButtonPressed = false;
		return;
	}
	if (!m_wasRightMouseButtonPressed) {
		m_wasRightMouseButtonPressed = true;
		m_input.GetMouseNormalizedPosition(m_oldXPosition, m_oldYPosition);
		return;
	}
	double currentXPosition, currentYPosition;

	m_input.GetMouseNormalizedPosition(currentXPosition, currentYPosition);

	const float sesitivity = 400;

	float yVelocity = (currentXPosition - m_oldXPosition) * sesitivity;
	//if (m_input.IsKeyDown(GLFW_KEY_UP))
	//	xVelocity = glm::radians(90.0f);
	//if (m_input.IsKeyDown(GLFW_KEY_DOWN))
	//	xVelocity = glm::radians(-90.0f);

	float xVelocity = -(currentYPosition - m_oldYPosition) * sesitivity;;
	//if (m_input.IsKeyDown(GLFW_KEY_RIGHT))
	//	yVelocity = glm::radians(90.0f);
	//if (m_input.IsKeyDown(GLFW_KEY_LEFT))
	//	yVelocity = glm::radians(-90.0f);

	m_oldXPosition = currentXPosition;
	m_oldYPosition = currentYPosition;

	glm::quat velocityQuaternion = glm::quat(0.0f, glm::vec3(xVelocity, yVelocity, 0.0f));

	m_orientationQuaternion += 0.5f * static_cast<float>(deltaTime) * velocityQuaternion * m_orientationQuaternion;
	m_orientationQuaternion = glm::normalize(m_orientationQuaternion);
}