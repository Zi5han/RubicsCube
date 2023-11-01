#include "TestObjCubie.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "OBJModel.h"

void TestObjCubie::Initialize() {
	m_objCubieRenderer.Initialize();
	m_turningAngle = 0.0f;
}

void TestObjCubie::Render(float aspectRatio) {
	glm::mat4 globalTransformation = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 1000.0f) *
		glm::lookAt(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(glm::mat4(1.0f), m_turningAngle, glm::vec3(1.0f, 1.0f, 1.0f));

	m_objCubieRenderer.Render(globalTransformation);
}

void TestObjCubie::ClearResources() {
	m_objCubieRenderer.ClearResources();
}

void TestObjCubie::Update(double deltaTime) {
	m_turningAngle += glm::radians(120.0f) * (static_cast<float>(deltaTime));
}
