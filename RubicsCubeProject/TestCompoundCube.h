#pragma once
#include "GameInterface.h"
#include "ObjCubieRenderer.h"
#include "InputSystem.h"
#include <glm/ext/quaternion_float.hpp>

class TestCompoundCube : public GameInterface {
public:
	virtual void Initialize(GLFWwindow* window);
	virtual void Render(float aspectRatio);
	virtual void ClearResources();
	virtual void Update(double deltaTime);

private:
	InputSystem m_input;
	ObjCubieRenderer m_cubieRenderer;
	float m_turningAngle;
	glm::quat m_orientationQuaternion;

};