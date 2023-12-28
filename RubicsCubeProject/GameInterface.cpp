#include "GameInterface.h"
#include "CubieRenderer.h"

void GameInterface::Initialize(GLFWwindow* window) {
	m_radius = 18.0;

	m_input.Initialize(window);

	// Initialize components
	m_rubicsCube.Initialize();
}

void GameInterface::RenderInterface(float aspectRatio) {
	// Recalculate matrices
	if (m_recalculationNeeded || aspectRatio != m_aspectRatio) {
		m_aspectRatio = aspectRatio;

		m_projection = glm::perspective(glm::radians(45.0f), m_aspectRatio, 0.1f, 100.0f);
		m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, m_radius), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		m_input.SetViewProjection(m_projection * m_view);
		m_recalculationNeeded = false;
	}
	// Render cube
	m_rubicsCube.Render(m_projection * m_view);
}

void GameInterface::Update(double deltaTime) {
	m_input.Update();

	// Update components
	m_rubicsCube.Update(*this);

	// Update camera distance
	m_radius -= m_input.GetMouseWheelScrollOffset().y;
	if (m_radius < 10)
		m_radius = 10;
	if (m_radius > 40)
		m_radius = 40;
	m_recalculationNeeded = true;
}

void GameInterface::ClearResources() {
	m_rubicsCube.ClearResources();
}

