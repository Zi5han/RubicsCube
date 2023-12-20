#pragma once
#include "GameInterface.h"
#include <glm/vec3.hpp>
#include "InputSystem.h"
#include "SimpleCubieRenderer.h"

class TestMouse : public GameInterface {
public:
	virtual void Initialize(GLFWwindow* window) override;
	virtual void RenderInterface(float aspectRatio) override;
	virtual void Update(double deltaTime) override;
	virtual void ClearResources() override;

private:
	SimpleCubieRenderer m_cubieRenderer;
	InputSystem m_input;
	glm::vec3 m_cubePosition;
	glm::mat4 m_viewProjection;
};