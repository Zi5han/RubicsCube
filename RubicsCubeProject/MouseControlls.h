#pragma once
#include "GameInterface.h"
#include <glm/vec3.hpp>
#include "InputSystem.h"
#include "CubieRenderer.h"

class MouseControlls : public GameInterface {
public:
	virtual void Initialize(GLFWwindow* window);
	virtual void Render(float aspectRatio);
	virtual void ClearResources();
	virtual void Update(double deltaTime);

private:
	CubieRenderer m_cubieRenderer;
	InputSystem m_input;
	glm::vec3 m_cubePosition;
	glm::mat4 m_viewProject;
	glm::vec2 m_actiualPosition;
	//glm::vec3 m_deltaMousePosition;
	//glm::vec3 m_oldMousePosition;

	bool m_wasRightMouseButtonPressed;
	glm::dvec2 m_oldPosition;
	glm::vec2 m_cameraVelocity;
	float m_cameraHeight;
};