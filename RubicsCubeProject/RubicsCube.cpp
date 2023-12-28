#include "RubicsCube.h"
#include "InputSystem.h"
#include "GameInterface.h"
#include <glm/gtx/intersect.hpp>

void RubicsCube::Initialize() {
	m_cubieRenderer.Initialize();
	d_lr.Initialize();
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
	d_lr.Render3D(d_gameInterface->GetProjectionMatrix(),
		d_gameInterface->GetViewMatrix(),
		glm::mat4(1.0f),
		glm::vec3(0.0f),
		d_endPoint,
		glm::vec3(1.0f, 0.0f, 0.0f));
}

void RubicsCube::Update(const GameInterface& gameInterface) {
	d_gameInterface = &gameInterface;
	const InputSystem& inputSystem = gameInterface.GetInputComponent();

	// CUBE ROTATION
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
	else if (inputSystem.GetActiveMouseButton() == InputSystem::LEFT_BUTTON) {
		// DETERMINE CLICKED FACE
		glm::vec3 origin, direction;
		inputSystem.GetPickingRay(origin, direction);

		if (inputSystem.GetLeftClickState() == InputSystem::HOLD) {
			for (auto normal : NORMALS_OF_FACES) {
				const glm::vec3 faceNormal = glm::mat3_cast(m_modelRotation) * normal.second;
				if (glm::dot(direction, faceNormal) > 0)
					continue;
				float intersectionDistance;
				bool intersects = glm::intersectRayPlane(origin, direction, faceNormal * (1.5f + m_cubieRenderer.GetCubieExtention()), faceNormal, intersectionDistance);
				if (!intersects)
					continue;
				glm::vec3 intersectionPoint = origin + intersectionDistance * direction;
				glm::vec3 intersectionPointInObjectSpace = glm::inverse(glm::mat3_cast(m_modelRotation)) * intersectionPoint;
				// 0.1f is tolerance
				if ((intersectionPointInObjectSpace.x > (1.5f + m_cubieRenderer.GetCubieExtention() + 0.1f) ||
					intersectionPointInObjectSpace.x	< -(1.5f + m_cubieRenderer.GetCubieExtention() + 0.1f) ||
					intersectionPointInObjectSpace.y	>(1.5f + m_cubieRenderer.GetCubieExtention() + 0.1f) ||
					intersectionPointInObjectSpace.y	< -(1.5f + m_cubieRenderer.GetCubieExtention() + 0.1f) ||
					intersectionPointInObjectSpace.z	>(1.5f + m_cubieRenderer.GetCubieExtention() + 0.1f) ||
					intersectionPointInObjectSpace.z < -(1.5f + m_cubieRenderer.GetCubieExtention() + 0.1f)))
					continue;
				m_clickedFace = normal.first;
				std::cout << m_clickedFace << ' ' <<
					intersectionPointInObjectSpace.x << ' ' <<
					intersectionPointInObjectSpace.y << ' ' <<
					intersectionPointInObjectSpace.z << '\n';
				d_endPoint = intersectionPoint;
				break;
			}
		}

	}
	// Safe previous screen position
	m_previousScreenPosition = inputSystem.GetScreenPosition();
}

void RubicsCube::ClearResources() {
	m_cubieRenderer.ClearResources();
}
