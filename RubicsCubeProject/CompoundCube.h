#pragma once
#include "GameInterface.h"
#include "CubieRenderer.h"
#include "InputSystem.h"
#include <glm/ext/quaternion_float.hpp>

class CompoundCube : public GameInterface {
public:
	virtual void Initialize(GLFWwindow* window);
	virtual void Render(float aspectRatio);
	virtual void ClearResources();
	virtual void Update(double deltaTime);

private:
	InputSystem m_input;
	CubieRenderer m_cubieRenderer;
	float m_turningAngle;
	glm::quat m_orientationQuaternion;

	bool m_wasRightMouseButtonPressed;
	double m_oldXPosition;
	double m_oldYPosition;
};