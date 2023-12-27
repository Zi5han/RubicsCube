#pragma once
#include "AbstractGameInterface.h"
#include <glm/vec3.hpp>
#include "InputSystem.h"
#include "SimpleCubieRenderer.h"
#include "CubieRenderer.h"
#include "LineRenderer.h"

class TestMouse : public AbstractGameInterface {
public:
	virtual void Initialize(GLFWwindow* window) override;
	virtual void RenderInterface(float aspectRatio) override;
	virtual void Update(double deltaTime) override;
	virtual void ClearResources() override;

private:
	CubieRenderer m_cubieRenderer;
	LineRenderer m_line;
	InputSystem m_input;

	glm::vec3 m_cubePosition;
	glm::mat4 m_projectionView;
};