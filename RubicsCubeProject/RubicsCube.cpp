#include "RubicsCube.h"
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
		glm::mat3_cast(m_modelRotation),
		glm::vec3(0.0f, 0.0f, 0.0f),
		d_endPoint,
		glm::vec3(1.0f, 0.0f, 0.0f));
}

void RubicsCube::Update(const GameInterface& gameInterface) {
	if (m_animationState == SNAPING) {}
	//UpdateAnimtion();


	d_gameInterface = &gameInterface;
	m_inputSystem = &gameInterface.GetInputComponent();

	if (m_inputSystem->GetActiveMouseButton() == InputSystem::RIGHT_BUTTON) {
		if (m_inputSystem->GetRightClickState() == InputSystem::HOLD) {
			h_RotateCube(gameInterface);
		}
	}
	else if (m_inputSystem->GetActiveMouseButton() == InputSystem::LEFT_BUTTON) {
		if (m_inputSystem->GetLeftClickState() == InputSystem::CLICK) {
			if (!m_animationState == SNAPING) {

				h_DetermineClickedFace(gameInterface);
				m_animationState = ROTATING;
			}
		}
		if (m_inputSystem->GetLeftClickState() == InputSystem::RELEASE) {
			m_animationState = SNAPING;
			return;
		}
	}
	// Safe previous screen position
	m_previousScreenPosition = m_inputSystem->GetScreenPosition();
}

void RubicsCube::ClearResources() {
	m_cubieRenderer.ClearResources();
}

//HELPING METHODS
void RubicsCube::h_RotateCube(const GameInterface& gameInterface) {
	// Calculate the difference between the current and previous screen positions
	glm::vec2 delta = m_inputSystem->GetScreenPosition() - m_previousScreenPosition;

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

void RubicsCube::h_DetermineClickedFace(const GameInterface& gameInterface) {
	glm::vec3 origin, direction;
	m_inputSystem->GetPickingRay(origin, direction);

	//Potentielle
	float planeOffset = 1.5f + m_cubieRenderer.GetCubieExtention();
	for (auto normal : NORMALS_OF_FACES) {
		const glm::vec3 faceNormal = glm::mat3_cast(m_modelRotation) * normal.second;
		if (glm::dot(direction, faceNormal) > 0)
			continue;

		float intersectionDistance;
		bool intersects = glm::intersectRayPlane(origin,
			direction,
			faceNormal * (planeOffset),
			faceNormal,
			intersectionDistance);
		if (!intersects)
			continue;

		glm::vec3 intersectionPoint = origin + intersectionDistance * direction;
		glm::vec3 intersectionPointInObjectSpace
			= glm::inverse(glm::mat3_cast(m_modelRotation)) * intersectionPoint;
		// 0.1f is tolerance
		if ((intersectionPointInObjectSpace.x > (planeOffset + 0.1f) ||
			intersectionPointInObjectSpace.x < -(planeOffset + 0.1f) ||
			intersectionPointInObjectSpace.y >(planeOffset + 0.1f) ||
			intersectionPointInObjectSpace.y < -(planeOffset + 0.1f) ||
			intersectionPointInObjectSpace.z >(planeOffset + 0.1f) ||
			intersectionPointInObjectSpace.z < -(planeOffset + 0.1f)))
			continue;

		m_clickedFace = normal.first;
		std::cout << m_clickedFace << ' ' <<
			d_endPoint.x << ' ' <<
			d_endPoint.y << ' ' <<
			d_endPoint.z << '\n';
		d_endPoint = intersectionPointInObjectSpace;
		break;
	}
}

//STATIC
const std::map<RubicsCube::Face, glm::vec3> RubicsCube::NORMALS_OF_FACES = {
	   {RubicsCube::FRONT, glm::vec3(0.0f, 0.0f, 1.0f)},
	   {RubicsCube::RIGHT, glm::vec3(1.0f, 0.0f, 0.0f)},
	   {RubicsCube::TOP, glm::vec3(0.0f, 1.0f, 0.0f)},
	   {RubicsCube::BACK, glm::vec3(0.0f, 0.0f, -1.0f)},
	   {RubicsCube::LEFT, glm::vec3(-1.0f, 0.0f, 0.0f)},
	   {RubicsCube::BOTTOM, glm::vec3(0.0f, -1.0f, 0.0f)}
};