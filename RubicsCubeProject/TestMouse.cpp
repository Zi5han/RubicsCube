#include "TestMouse.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

void TestMouse::Initialize(GLFWwindow* window)
{
	m_input.SetWindow(window);
	m_cubePosition = glm::vec3(0.0f);
	m_viewProjection = glm::mat4(1.0f);
	m_cubieRenderer.Initialize();
}

void TestMouse::RenderInterface(float aspectRatio)
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -9.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_viewProjection = projection * view;
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_cubePosition);

	m_cubieRenderer.Render(projection, view, model);
}

void TestMouse::ClearResources()
{
	m_cubieRenderer.ClearResources();
}

void TestMouse::Update(double deltaTime)
{
	if (m_input.GetLeftClickState() == InputSystem::HOLD)
	{
		glm::vec3 position, direction;
		m_input.SetViewProjection(m_viewProjection);
		m_input.GetPickingRay(position, direction);
		m_cubePosition = position + 6.0f * direction;
	}
}