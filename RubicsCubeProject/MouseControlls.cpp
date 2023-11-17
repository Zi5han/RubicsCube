#include "MouseControlls.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

void MouseControlls::Initialize(GLFWwindow* window)
{
	m_input.SetWindow(window);
	m_cubePosition = glm::vec3(0.0f);
	m_viewProject = glm::mat4(1.0f);
	m_deltaMousePosition = glm::vec3(0.0f);
	m_oldMousePosition = glm::vec3(0.0f);
	m_cubieRenderer.Initialize();
}

void MouseControlls::Render(float aspectRatio) {

	//glm::vec3 cameraPosition = glm::vec3((cosf(m_deltaMousePosition.x)) * 18.0f, 
	// sinf(m_deltaMousePosition.x) * 18.0f, 
	// sinf(m_deltaMousePosition.x) * 18.0f);
	// 
	//glm::normalize(cameraPosition);
	//cameraPosition = cameraPosition * 18.0f;

	float radius = 18.0f;
	float cameraHeight = sinf(m_deltaMousePosition.y) * radius;
	
	//glm::vec3 cameraPosition = glm::vec3(
	//	cosf(m_deltaMousePosition.x) * cameraHeight,
	//	sinf(m_deltaMousePosition.x) * cameraHeight,
	//	cosf(m_deltaMousePosition.y) * radius
	//);

	float horizontalDistance = cosf(m_deltaMousePosition.y) * radius;
	
	glm::vec3 cameraPosition = glm::vec3(
		horizontalDistance * cosf(m_deltaMousePosition.x),
		horizontalDistance * sinf(m_deltaMousePosition.x),
		cameraHeight
	);

	m_viewProject = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f) *
		glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 finalMat = glm::translate(m_viewProject, m_cubePosition);

	m_cubieRenderer.Render(finalMat);
}

void MouseControlls::ClearResources() {
	m_cubieRenderer.ClearResources();
}

void MouseControlls::Update(double deltaTime) {
	//if (!m_input.IsLeftMouseButtonDown()) {
	//	m_deltaMousePosition = glm::vec3(0.0f);
	//	return;
	//}

	//glm::vec3 position(0), direction(0);
    //m_input.GetPickingRay(m_viewProject, position, direction);

	//m_deltaMousePosition = (position - m_oldMousePosition);

	//m_cubePosition = position + 18.0f * direction;

	//m_deltaMousePosition = position;

	m_deltaMousePosition.y += 0.02f;
	m_deltaMousePosition.x += 0.01f;



	
}