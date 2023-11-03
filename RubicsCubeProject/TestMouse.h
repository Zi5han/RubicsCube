#pragma once
#include "GameInterface.h"
#include <glm/vec3.hpp>
#include "InputSystem.h"
#include "ObjCubieRenderer.h"

class TestMouse : public GameInterface
{
public:
	virtual void Initialize(GLFWwindow* window);
	virtual void Render(float aspectRatio);
	virtual void ClearResources();
	virtual void Update(double deltaTime);

private:
	ObjCubieRenderer m_cubieRenderer;
	InputSystem m_input;
	glm::vec3 m_cubePosition;
	glm::mat4 m_viewProject;
};