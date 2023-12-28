#include "GameInterface.h"
#include "CubieRenderer.h"

void GameInterface::Initialize(GLFWwindow* window) {
	m_input.Initialize(window);
	m_rubicsCube.Initialize();
}

void GameInterface::RenderInterface(float aspectRatio) {
	if (m_recalculationNeeded || aspectRatio != m_aspectRatio) {
		m_aspectRatio = aspectRatio;
		RecalculateMatrices(m_aspectRatio);
	}
	m_rubicsCube.Render(m_projection * m_view);
}

void GameInterface::Update(double deltaTime) {
	m_input.Update();

	m_rubicsCube.Update(deltaTime);
}

void GameInterface::ClearResources() {
	m_rubicsCube.ClearResources();
}

void GameInterface::RecalculateMatrices(float aspectRatio) {
	m_projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 18.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_recalculationNeeded = false;
}

