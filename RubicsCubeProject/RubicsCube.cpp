#include "RubicsCube.h"
#include "GameInterface.h"
#include <glm/gtx/intersect.hpp>

void RubicsCube::Initialize() {
	m_cubieRenderer.Initialize();
	d_lr.Initialize();

	float offset = m_cubieRenderer.GetCubieExtention();

	for (int z = 0; z < 3; z++)
		for (int y = 0; y < 3; y++)
			for (int x = 0; x < 3; x++) {
				m_grid[x][y][z] = new Cubie;
				m_grid[x][y][z]->m_number = x + y + z;
				m_grid[x][y][z]->m_position = glm::vec3((x - 1) * offset, (y - 1) * offset, (z - 1) * offset);
			}

}

void RubicsCube::Render(const glm::mat4& viewProjection) {
	//Offset + 0.1f damit die Luecken zwischen den Minicubies erscheinen.
	glm::mat4 cubeRotation = glm::mat4_cast(m_modelRotation);

	for (int z = 0; z < 3; z++) {
		for (int y = 0; y < 3; y++) {
			for (int x = 0; x < 3; x++) {
				glm::mat4 cubieTranslation = glm::translate(glm::mat4(1.0f), m_grid[x][y][z]->m_position);
				glm::mat4 cubieRotation = m_grid[x][y][z]->m_visibleRotation;
				m_cubieRenderer.Render(viewProjection, cubeRotation * cubieRotation * cubieTranslation);
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
	//Animation
	if (m_animationState == SNAPING) {
		h_UpdateAnimation(gameInterface);
	}

	d_gameInterface = &gameInterface;

	h_UpdateMouse(gameInterface);
	h_UpdateKeyInput(gameInterface);

	m_previousScreenPosition = inputSystem->GetScreenPosition();
}

void RubicsCube::h_UpdateMouse(const GameInterface& gameInterface)
{
	inputSystem = &gameInterface.GetInputComponent();

	if (inputSystem->GetActiveMouseButton() == InputSystem::RIGHT_BUTTON) {
		if (inputSystem->GetRightClickState() == InputSystem::HOLD) {
			h_RotateCube(gameInterface);
		}
	}
	else if (inputSystem->GetActiveMouseButton() == InputSystem::LEFT_BUTTON) {
		switch (inputSystem->GetLeftClickState()) {

		case InputSystem::CLICK:
			if (m_animationState != STABLE)
				break;
			h_DetermineClickedFace(gameInterface);
			m_animationState = ROTATING;
			break;

		case InputSystem::HOLD:
			if (m_animationState == SNAPING)
				break;
			if (m_animationState == ROTATING) {
				if (m_activeFaceSlice == UNSET_SLICE)
					if (glm::length(inputSystem->GetScreenPosition() - inputSystem->GetDragStartScreenPosition()) > 100) {
						h_DetermineActiveFace(gameInterface);
						break;
					}
				h_RotateFace(gameInterface);
			}
			break;

		case InputSystem::RELEASE:
			m_animationState = SNAPING;
			break;

		default:
			break;

		}
	}
}

void RubicsCube::h_UpdateKeyInput(const GameInterface& gameInterface) {
	// TODO
}

void RubicsCube::ClearResources() {
	m_cubieRenderer.ClearResources();
	for (auto& arr2d : m_grid)
		for (auto& arr : arr2d)
			for (auto& e : arr)
				delete e;
}

//HELPING METHODS
void RubicsCube::h_RotateCube(const GameInterface& gameInterface) {
	// Calculate the difference between the current and previous screen positions
	glm::vec2 delta = inputSystem->GetScreenPosition() - m_previousScreenPosition;

	// Convert the delta vector to radians
	float angleX = glm::radians(delta.x);
	float angleY = glm::radians(delta.y);

	// Create quaternions for the rotations
	glm::quat rotationX = glm::angleAxis(angleX, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat rotationY = glm::angleAxis(angleY, glm::vec3(1.0f, 0.0f, 0.0f));

	// Apply the rotations to the cubeRotation
	m_modelRotation = rotationX * m_modelRotation;
	m_modelRotation = rotationY * m_modelRotation;
	m_modelRotation = glm::normalize(m_modelRotation);
}

void RubicsCube::h_DetermineClickedFace(const GameInterface& gameInterface) {
	glm::vec3 origin, direction;
	inputSystem->GetPickingRay(origin, direction);

	float planeOffset = 1.5f * m_cubieRenderer.GetCubieExtention();
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
		m_rayCubeIntersectionPoint = intersectionPoint;
		d_endPoint = intersectionPointInObjectSpace;
		//std::cout << m_clickedFace << ' ' <<
		//	d_endPoint.x << ' ' <<
		//	d_endPoint.y << ' ' <<
		//	d_endPoint.z << '\n';
		break;
	}
}

void RubicsCube::h_RotateFace(const GameInterface& gameInterface) {

}

void RubicsCube::h_DetermineActiveFace(const GameInterface& gameInterface) {
	glm::vec3 intersectionPointInObjectSpace
		= glm::inverse(glm::mat3_cast(m_modelRotation)) * m_rayCubeIntersectionPoint;
	float planeOffset = 1.5f * m_cubieRenderer.GetCubieExtention();

	//int activeSliceX = static_cast<int>((intersectionPointInObjectSpace.x));
	//int activeSliceY = static_cast<int>((intersectionPointInObjectSpace.x));
	//int activeSliceZ = static_cast<int>((intersectionPointInObjectSpace.x));

	//float activeSliceX = intersectionPointInObjectSpace.x;
	//float activeSliceY = intersectionPointInObjectSpace.y;
	//float activeSliceZ = intersectionPointInObjectSpace.z;
	//
	//if (m_clickedFace == TOP_FACE || m_clickedFace == BOTTOM_FACE) {
	//	//m_activeCube = activeSliceX;
	//}
	//else if (m_clickedFace == LEFT_FACE || m_clickedFace == RIGHT_FACE) {
	//	//m_activeCube = activeSliceY;
	//}
	//else { // m_clickedFace == FRONT || m_clickedFace == BACK
	//	//m_activeCube = activeSliceZ;
	//}
	//
	//std::cout << activeSliceX << ' ' << activeSliceY << ' ' << activeSliceZ << ' ' << '\n';
}

void RubicsCube::h_UpdateAnimation(const GameInterface& gameInterface) {
	m_animationState = STABLE;
}

//STATIC
const std::map<RubicsCube::CubeFace, glm::vec3> RubicsCube::NORMALS_OF_FACES = {
	   {RubicsCube::RIGHT_FACE,		glm::vec3(1.0f, 0.0f, 0.0f)},
	   {RubicsCube::TOP_FACE,		glm::vec3(0.0f, 1.0f, 0.0f)},
	   {RubicsCube::FRONT_FACE,		glm::vec3(0.0f, 0.0f, 1.0f)},
	   {RubicsCube::LEFT_FACE,		glm::vec3(-1.0f, 0.0f, 0.0f)},
	   {RubicsCube::BOTTOM_FACE,	glm::vec3(0.0f, -1.0f, 0.0f)},
	   {RubicsCube::BACK_FACE,		glm::vec3(0.0f, 0.0f, -1.0f)}
};

const std::map<RubicsCube::CubeSlice, glm::vec3> RubicsCube::NORMALS_OF_SLICES = {
	   {RubicsCube::RIGHT_SLICE,	glm::vec3(1.0f, 0.0f, 0.0f)},
	   {RubicsCube::TOP_SLICE,		glm::vec3(0.0f, 1.0f, 0.0f)},
	   {RubicsCube::FRONT_SLICE,	glm::vec3(0.0f, 0.0f, 1.0f)},
	   {RubicsCube::LEFT_SLICE,		glm::vec3(1.0f, 0.0f, 0.0f)},
	   {RubicsCube::BOTTOM_SLICE,	glm::vec3(0.0f, 1.0f, 0.0f)},
	   {RubicsCube::BACK_SLICE,		glm::vec3(0.0f, 0.0f, 1.0f)},
	   {RubicsCube::X_MID_SLICE,	glm::vec3(1.0f, 0.0f, 0.0f)},
	   {RubicsCube::Y_MID_SLICE,	glm::vec3(0.0f, 1.0f, 0.0f)},
	   {RubicsCube::Z_MID_SLICE,	glm::vec3(0.0f, 0.0f, 1.0f)},
};