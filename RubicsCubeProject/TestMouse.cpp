#include "TestMouse.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

void TestMouse::Initialize(GLFWwindow* window)
{
	m_input.SetWindow(window);
	m_cubePosition = glm::vec3(0.0f);
	m_viewProjection = glm::mat4(1.0f);
	m_cubieRenderer.Initialize();
	m_line.Initialize();
}

void TestMouse::RenderInterface(float aspectRatio)
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, -9.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_viewProjection = projection * view;
	glm::mat4 model = glm::translate(glm::mat4(1.0f), m_cubePosition);

	m_cubieRenderer.Render(projection, view, model);
	//m_line.Render3D(projection, view, glm::mat4(1.0f), glm::vec3(0.0f), m_cubePosition, glm::vec3(1.0f, 0.0f, 0.0f));
	//m_line.Render2D(projection, view, glm::mat4(1.0f), glm::vec2(0.5f), m_input.GetMousePosition(), glm::vec3(0.0f, 1.0f, 0.0f));
	m_line.Render2D(m_input.GetDragStart2DMousePosition(true), m_input.GetMouse2DPosition(true), glm::vec3(0.0f, 1.0f, 0.0f));


}

void TestMouse::ClearResources()
{
	m_cubieRenderer.ClearResources();
}

void TestMouse::Update(double deltaTime)
{
	m_input.FetchInputs();
	if (m_input.GetLeftClickState() == InputSystem::HOLD)
	{
		glm::vec3 position, direction;
		m_input.SetViewProjection(m_viewProjection);
		m_input.GetPickingRay(position, direction);
		m_cubePosition = position + 9.0f * direction;
	}
}