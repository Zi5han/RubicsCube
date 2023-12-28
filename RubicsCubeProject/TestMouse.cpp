#include "TestMouse.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

void TestMouse::Initialize(GLFWwindow* window) {
	m_input.Initialize(window);
	m_cubieRenderer.Initialize();
	m_line.Initialize();

	m_cubePosition = glm::vec3(0.0f);
	m_viewProjection = glm::mat4(1.0f);
}

void TestMouse::RenderInterface(float aspectRatio) {
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -9.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_viewProjection = projection * view;
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_cubePosition);

	m_cubieRenderer.Render(m_viewProjection, model);
	glm::vec3 dragStart_origin, origin, direction;
	m_input.GetDragStartPickingRay(dragStart_origin, direction);
	m_input.GetPickingRay(origin, direction);

	m_line.Render3D(projection, view, glm::mat4(1.0f), dragStart_origin, origin, glm::vec3(1.0f, 0.0f, 0.0f));
	m_line.Render2D(m_input.NormalizeScreenVector(m_input.GetDragStartScreenPosition()), m_input.NormalizeScreenVector(m_input.GetScreenPosition()), glm::vec3(0.0f, 1.0f, 0.0f));
}

void TestMouse::ClearResources() {
	m_cubieRenderer.ClearResources();
}

void TestMouse::Update(double deltaTime) {
	m_input.Update();
	if (m_input.GetLeftClickState() == InputSystem::HOLD)
	{
		glm::vec3 position, direction;
		m_input.GetPickingRay(position, direction);
		m_cubePosition = position + 9.0f * direction;
	}
	m_input.SetViewProjection(m_viewProjection);
}