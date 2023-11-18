#include "MouseControlls.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

void MouseControlls::Initialize(GLFWwindow* window)
{
	m_input.SetWindow(window);
	m_cubePosition = glm::vec3(0.0f);
	m_viewProject = glm::mat4(1.0f);
	//m_deltaMousePosition = glm::vec3(0.0f);
	//m_oldMousePosition = glm::vec3(0.0f);
	m_cubieRenderer.Initialize();
	m_oldPosition = glm::vec2(0, 0);
	m_cameraVelocity = glm::vec2(0, 0);
}

void MouseControlls::Render(float aspectRatio) {

	//glm::vec3 cameraPosition = glm::vec3((cosf(m_deltaMousePosition.x)) * 18.0f, 
	// sinf(m_deltaMousePosition.x) * 18.0f, 
	// sinf(m_deltaMousePosition.x) * 18.0f);
	// 
	//glm::normalize(cameraPosition);
	//cameraPosition = cameraPosition * 18.0f;

	float radius = 18.0f;
	m_actiualPosition.y = m_actiualPosition.y > 1 ? 1 : m_actiualPosition.y;
	m_actiualPosition.y = m_actiualPosition.y < -1 ? -1 : m_actiualPosition.y;
	float cameraHeight = sinf(m_actiualPosition.y);
	
	//glm::vec3 cameraPosition = glm::vec3(
	//	cosf(m_deltaMousePosition.x) * cameraHeight,
	//	sinf(m_deltaMousePosition.x) * cameraHeight,
	//	cosf(m_deltaMousePosition.y) * radius
	//);

	float horizontalDistance = cosf(m_actiualPosition.y);
	
	glm::vec3 cameraPosition = glm::vec3(
		horizontalDistance * cosf(m_actiualPosition.x),
		cameraHeight,
		horizontalDistance * sinf(m_actiualPosition.x)
	);

	cameraPosition *= radius;

	m_viewProject = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f) *
		glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 finalMat = glm::translate(m_viewProject, m_cubePosition);

	m_cubieRenderer.Render(finalMat);
}

void MouseControlls::ClearResources() {
	m_cubieRenderer.ClearResources();
}

//void MouseControlls::Update(double deltaTime) {
//	//if (!m_input.IsLeftMouseButtonDown()) {
//	//	m_deltaMousePosition = glm::vec3(0.0f);
//	//	return;
//	//}
//
//	//glm::vec3 position(0), direction(0);
//    //m_input.GetPickingRay(m_viewProject, position, direction);
//
//	//m_deltaMousePosition = (position - m_oldMousePosition);
//
//	//m_cubePosition = position + 18.0f * direction;
//
//	//m_deltaMousePosition = position;
//
//	m_deltaMousePosition.y += 0.02f;
//	m_deltaMousePosition.x += 0.01f;
//}

void MouseControlls::Update(double deltaTime) {
	m_input.Update();
	//if (m_input.IsKeyDown(GLFW_KEY_SPACE))
	//	//m_orientationQuaternion = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

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

	const float sesitivity = 10;

	float xVelocity = (currentXPosition - m_oldPosition.x) * sesitivity;
	//if (m_input.IsKeyDown(GLFW_KEY_UP))
	//	xVelocity = glm::radians(90.0f);
	//if (m_input.IsKeyDown(GLFW_KEY_DOWN))
	//	xVelocity = glm::radians(-90.0f);

	float yVelocity = (currentYPosition - m_oldPosition.y) * sesitivity;
	//if (m_input.IsKeyDown(GLFW_KEY_RIGHT))
	//	yVelocity = glm::radians(90.0f);
	//if (m_input.IsKeyDown(GLFW_KEY_LEFT))
	//	yVelocity = glm::radians(-90.0f);

	m_actiualPosition.x += xVelocity;
	m_actiualPosition.y += yVelocity;

	m_oldPosition.x = currentXPosition;
	m_oldPosition.y = currentYPosition;

	//std::cout << m_oldPosition.x << ' ' << m_oldPosition.y << currentXPosition << ' ' << currentYPosition << '\n';
	std::cout << currentXPosition << ' ' << currentYPosition << '\n';
	std::cout << '\n';


	//glm::quat velocityQuaternion = glm::quat(0.0f, glm::vec3(xVelocity, yVelocity, 0.0f));

	//m_orientationQuaternion += 0.5f * static_cast<float>(deltaTime) * velocityQuaternion * m_orientationQuaternion;
	//m_orientationQuaternion = glm::normalize(m_orientationQuaternion);
}