#pragma once
#include "AbstractGameInterface.h"
#include "InputSystem.h"
#include "RubicsCube.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class GameInterface : public AbstractGameInterface {
public:
	virtual void Initialize(GLFWwindow* window) override;
	virtual void RenderInterface(float aspectRatio) override;
	virtual void Update(double deltaTime) override;
	virtual void ClearResources() override;

	const InputSystem& GetInputComponent() const { return m_input; }
	const glm::mat4& GetProjectionMatrix() const { return m_projection; }
	const glm::mat4& GetViewMatrix() const { return m_view; }

	void QueueMatrixRecalculation() const { m_recalculationNeeded = true; }

private:
	InputSystem m_input;
	RubicsCube m_rubicsCube;

	float m_aspectRatio;

	mutable bool m_recalculationNeeded;
	float m_radius;

	//MATRICES
	glm::mat4 m_projection;
	glm::mat4 m_view;
};