#pragma once
#include "InputSystem.h"
#include "AbstractGameInterface.h"

struct GLFWwindow;

class TestSound : public AbstractGameInterface {
public:
	virtual void Initialize(GLFWwindow* winndow);

	virtual void Update(double deltaTime);

private:
	InputSystem m_input;
};