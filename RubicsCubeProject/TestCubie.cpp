#include "TestCubie.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

void TestCubie::Initialize() {
	m_cubieRenderer.Initialize();
	m_turningAngle = 0.0f;
}

void TestCubie::RenderInterface(float aspectRatio) {
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), m_turningAngle, glm::vec3(1.0f, 1.0f, 1.0f));

	glm::mat4 projectionView = projection * view;

	m_cubieRenderer.Render(projectionView, model);

}

void TestCubie::ClearResources() {
	m_cubieRenderer.ClearResources();
}

void TestCubie::Update(double deltaTime) {
	m_turningAngle += glm::radians(120.0f) * (static_cast<float>(deltaTime));
}
