#include "GameInterface.h"

void GameInterface::Initialize()
{
	m_cubieRenderer.Initialize();
	m_turningAngle = 0.0f;
}

void GameInterface::RenderInterface(float aspectRatio) {
	RecalculateMatrices(aspectRatio);
	glm::mat4 projectionView = m_projection * m_view;
}

void GameInterface::ClearResources() {
	m_cubieRenderer.ClearResources();
}

void GameInterface::RecalculateMatrices(float aspectRatio) {
	m_projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, -9.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_model = glm::rotate(glm::mat4(1.0f), m_turningAngle, glm::vec3(1.0f, 1.0f, 1.0f));
}

void GameInterface::Update(double deltaTime) {
	m_turningAngle += glm::radians(120.0f) * ((float)deltaTime);
}