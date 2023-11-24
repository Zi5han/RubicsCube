#pragma once
#include "InputSystem.h"

struct GLFWwindow;

class TestSound : public GameInterface {
public:
	virtual void Initialize(GLFWwindow* winndow);

	virtual void Update(double deltaTime) {}

private:
	InputSystem m_input;
};