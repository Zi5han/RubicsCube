#pragma once
#include "AbstractGameInterface.h"
#include "CubieRenderer.h"
#include "InputSystem.h"
#include <glm/ext/quaternion_float.hpp>

class TestKey : public AbstractGameInterface {
public:
	virtual void Initialize(GLFWwindow* window) override;
	virtual void RenderInterface(float aspectRatio) override;
	virtual void Update(double deltaTime) override;
	virtual void ClearResources() override;

private:
	CubieRenderer m_cubieRenderer;
	InputSystem m_input;
	glm::quat m_orientationQuaternion;
};