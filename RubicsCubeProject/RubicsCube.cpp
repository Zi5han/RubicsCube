#include "RubicsCube.h"
#include "InputSystem.h"
#include "GameInterface.h"

void RubicsCube::Initialize() {
	m_cubieRenderer.Initialize();
}

void RubicsCube::Render(const glm::mat4& viewProjection) {
	//Offset + 0.1f damit die Luecken zwischen den Minicubies erscheinen.
	float offset = m_cubieRenderer.GetCubieExtention() + 0.1f;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) {
				glm::mat4 model = glm::mat4_cast(m_modelRotation);
				model = glm::translate(model, glm::vec3((i - 1) * offset, (j - 1) * offset, (k - 1) * offset)); // then apply translation

				//Rotation der mittleren horizontalen auf jeder Seite
				//compound = glm::rotate(compound, glm::radians(90.0f) * (i % 2), glm::vec3(1.0f, 0.0f, 0.0f));
				//compound = glm::rotate(compound, glm::radians(90.0f) * (j % 2), glm::vec3(0.0f, 1.0f, 0.0f));
				//compound = glm::rotate(compound, glm::radians(90.0f) * (k % 2), glm::vec3(0.0f, 0.0f, 1.0f));
				m_cubieRenderer.Render(viewProjection, model);
			}
		}
	}
}

void RubicsCube::Update(const GameInterface& gameInterface) {
	const InputSystem& inputSystem = gameInterface.getInputComponent();

	// Cube rotation
	if (inputSystem.GetActiveMouseButton() == InputSystem::RIGHT_BUTTON
		&& inputSystem.GetRightClickState() == InputSystem::HOLD) {
		// Calculate the difference between the current and previous screen positions
		glm::vec2 delta = inputSystem.GetScreenPosition() - m_previousScreenPosition;

		// Convert the delta vector to radians
		float angleX = glm::radians(delta.x);
		float angleY = glm::radians(delta.y);

		// Create quaternions for the rotations
		glm::quat rotationX = glm::angleAxis(angleX, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat rotationY = glm::angleAxis(angleY, glm::vec3(1.0f, 0.0f, 0.0f));

		// Apply the rotations to the model
		m_modelRotation = rotationX * m_modelRotation;
		m_modelRotation = rotationY * m_modelRotation;
		m_modelRotation = glm::normalize(m_modelRotation);
	}

	// Safe previous screen position
	m_previousScreenPosition = inputSystem.GetScreenPosition();
}

void RubicsCube::ClearResources() {
	m_cubieRenderer.ClearResources();
}
