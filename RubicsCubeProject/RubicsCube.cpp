#include "RubicsCube.h"

void RubicsCube::Initialize() {
	m_cubieRenderer.Initialize();
}

void RubicsCube::Render(const glm::mat4& viewProjection) {
	//Offset + 0.1f damit die Luecken zwischen den Minicubies erscheinen.
	float offset = m_cubieRenderer.GetCubieExtention() + 0.1f;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3((i - 1) * offset, (j - 1) * offset, (k - 1) * offset));

				//Rotation der mittleren horizontalen auf jeder Seite
				//compound = glm::rotate(compound, glm::radians(90.0f) * (i % 2), glm::vec3(1.0f, 0.0f, 0.0f));
				//compound = glm::rotate(compound, glm::radians(90.0f) * (j % 2), glm::vec3(0.0f, 1.0f, 0.0f));
				//compound = glm::rotate(compound, glm::radians(90.0f) * (k % 2), glm::vec3(0.0f, 0.0f, 1.0f));
				m_cubieRenderer.Render(viewProjection, model);
			}
		}
	}
}

void RubicsCube::Update(float deltaTime) {

}

void RubicsCube::ClearResources() {
	m_cubieRenderer.ClearResources();
}
