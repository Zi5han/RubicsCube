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
	glm::vec2 m_actiualPosition;

	bool m_wasRightMouseButtonPressed;
	glm::dvec2 m_oldPosition;

};