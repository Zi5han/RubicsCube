#include "RubicsCube.h"
#include "GameInterface.h"
#include <glm/gtx/intersect.hpp>

void RubicsCube::Initialize(const GameInterface& gameInterface) {
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
	m_inputSystem = &gameInterface.GetInputComponent();

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
		//glm::mat3(1.0f)
		glm::vec3(0.0f),
		d_endPoint,
		glm::vec3(1.0f, 0.0f, 0.0f));
}

void RubicsCube::Update(const GameInterface& gameInterface) {
	//Animation
	if (m_animationState == AnimationState::SNAPING) {
		h_UpdateAnimation();
	}

	d_gameInterface = &gameInterface;

	h_UpdateMouse();
	h_UpdateKeyInput();

	m_previousScreenPosition = m_inputSystem->GetScreenPosition();
}

void RubicsCube::h_UpdateMouse()
{
	if (m_inputSystem->GetActiveMouseButton() == InputSystem::RIGHT_BUTTON) {
		if (m_inputSystem->GetRightClickState() == InputSystem::HOLD) {
			h_RotateCube();
		}
	}
	else if (m_inputSystem->GetActiveMouseButton() == InputSystem::LEFT_BUTTON) {
		switch (m_inputSystem->GetLeftClickState()) {

		case InputSystem::CLICK:
			if (m_animationState != AnimationState::STABLE)
				break;
			else {
				h_DetermineClickedFace();
			}
			break;

		case InputSystem::HOLD:
			if (m_animationState == AnimationState::SNAPING)
				break;
			else if (m_animationState == AnimationState::STABLE) {
				if (m_clickedFace == CubeFace::UNSET_FACE)
					break;
				if (glm::length(m_inputSystem->GetScreenPosition() - m_inputSystem->GetDragStartScreenPosition()) > 10) {
					h_DetermineActiveFace();
					m_animationState = AnimationState::ROTATING;
				}
				h_RotateFace();
			}
			break;

		case InputSystem::RELEASE:
			if (m_animationState != AnimationState::STABLE)
				m_animationState = AnimationState::SNAPING;
			break;

		default:
			break;

		}
	}
}

void RubicsCube::h_UpdateKeyInput() {
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
void RubicsCube::h_RotateCube() {
	// Calculate the difference between the current and previous screen positions
	glm::vec2 delta = m_inputSystem->GetScreenPosition() - m_previousScreenPosition;

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

void RubicsCube::h_DetermineClickedFace() {
	glm::vec3 origin, direction;
	m_inputSystem->GetPickingRay(origin, direction);

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
		m_facePlaneIntersectionPoint = intersectionPoint;
		//d_endPoint = intersectionPointInObjectSpace;
		//std::cout << m_clickedFace << ' ' <<
		//	d_endPoint.x << ' ' <<
		//	d_endPoint.y << ' ' <<
		//	d_endPoint.z << '\n';
		break;
	}
}

void RubicsCube::h_RotateFace() {

}

void RubicsCube::h_DetermineActiveFace() {
	// Es wird der Schnittpunkt in Object Space bestimmt
	glm::vec3 intersectionPointInObjectSpace
		= glm::inverse(glm::mat3_cast(m_modelRotation)) * m_facePlaneIntersectionPoint;
	float planeOffset = 1.5f * m_cubieRenderer.GetCubieExtention();

	// Die slice indices werden bestimmt
	m_xSliceIndex
		= (intersectionPointInObjectSpace.x >= (-planeOffset / 3)
			&& intersectionPointInObjectSpace.x <= (planeOffset / 3))
		? 1 : (intersectionPointInObjectSpace.x > (planeOffset / 3) ? 2 : 0);
	m_ySliceIndex
		= (intersectionPointInObjectSpace.y >= (-planeOffset / 3)
			&& intersectionPointInObjectSpace.y <= (planeOffset / 3))
		? 1 : (intersectionPointInObjectSpace.y > (planeOffset / 3) ? 2 : 0);
	m_zSliceIndex
		= (intersectionPointInObjectSpace.z >= (-planeOffset / 3)
			&& intersectionPointInObjectSpace.z <= (planeOffset / 3))
		? 1 : (intersectionPointInObjectSpace.z > (planeOffset / 3) ? 2 : 0);



	// Es wird der aktuelle Schnittpunkt auf der geklicken Ebene im Object Space bestimmt
	float intersectionDistance;
	glm::vec3 origin, direction;
	m_inputSystem->GetPickingRay(origin, direction);
	//Warnung: bei freier Kamerasteurung, könnte es auch kein Schnittpunkt geben
	// Hier wird die Normale in für die Rechnung rotiert, dannach wird die Rotation rückgängig gemacht
	glm::intersectRayPlane(origin,
		direction,
		glm::mat3_cast(m_modelRotation) * NORMALS_OF_FACES.at(m_clickedFace) * (planeOffset),
		glm::mat3_cast(m_modelRotation) * NORMALS_OF_FACES.at(m_clickedFace),
		intersectionDistance);
	glm::vec3 currenIntersectionPointInObjectSpace =
		glm::inverse(glm::mat3_cast(m_modelRotation)) * (origin + intersectionDistance * direction);

	// Normale der geklicken Face wird normalisiert
	int positiveNormalIndex = static_cast<int>(m_clickedFace) % 3;

	// Die Drag Richtung wird bestimmt
	glm::vec3 dragDirection = currenIntersectionPointInObjectSpace - intersectionPointInObjectSpace;
	glm::vec3 dragDirectionNormal = h_findClosestDirection(
		dragDirection,
		NORMALS_OF_FACES.at(static_cast<CubeFace>((positiveNormalIndex + 1) % 3)),
		NORMALS_OF_FACES.at(static_cast<CubeFace>((positiveNormalIndex + 2) % 3)));

	//d_endPoint = dragDirection * 10.0f;

	if (m_clickedFace == CubeFace::LEFT_FACE || m_clickedFace == CubeFace::RIGHT_FACE) {
		if (dragDirectionNormal.y == 1.0f) {
			m_activeFaceNormal = Axis::Z;
		}
		else if (dragDirectionNormal.z == 1.0f) {
			m_activeFaceNormal = Axis::Y;
		}
	}
	else if (m_clickedFace == CubeFace::FRONT_FACE || m_clickedFace == CubeFace::BACK_FACE) {
		if (dragDirectionNormal.x == 1.0f) {
			m_activeFaceNormal = Axis::Y;
		}
		else if (dragDirectionNormal.y == 1.0f) {
			m_activeFaceNormal = Axis::X;
		}
	}
	else { // m_clickedFace == TOP_FACE || m_clickedFace == BOTTOM_FACE
		if (dragDirectionNormal.x == 1.0f) {
			m_activeFaceNormal = Axis::Z;
		}
		else if (dragDirectionNormal.z == 1.0f) {
			m_activeFaceNormal = Axis::X;
		}
	}

	std::cout << "Der Slice ist "
		<< "X: " << m_xSliceIndex << ", "
		<< "Y: " << m_ySliceIndex << ", "
		<< "Z: " << m_zSliceIndex << '\n';
	std::cout << "Die aktive Achse ist: " << static_cast<int>(m_activeFaceNormal) << '\n';


	//std::cout << activeSliceX << ' ' << activeSliceY << ' ' << activeSliceZ << ' ' << '\n';
	//std::cout << intersectionPointInObjectSpace.x << ' ' << intersectionPointInObjectSpace.y << ' ' << intersectionPointInObjectSpace.z << ' ' << '\n';

}

glm::vec3 RubicsCube::h_findClosestDirection(const glm::vec3& referenceDirection, const glm::vec3& vecU, const glm::vec3& vecV) {
	float dotProductU = glm::dot(referenceDirection, vecU);
	float dotProductV = glm::dot(referenceDirection, vecV);
	float dotProductNegativeU = glm::dot(referenceDirection, -vecU);
	float dotProductNegativeV = glm::dot(referenceDirection, -vecV);

	float maxDotProduct = std::max({ dotProductU, dotProductV, dotProductNegativeU, dotProductNegativeV });

	if (maxDotProduct == dotProductU)
		return vecU;

	else if (maxDotProduct == dotProductV)
		return vecV;

	else if (maxDotProduct == dotProductNegativeU)
		return vecU;

	else // maxDotProduct == dotProductNegativeV 
		return vecV;

}

void RubicsCube::h_UpdateAnimation() {
	m_animationState = AnimationState::STABLE;
}

//STATIC
const std::map<RubicsCube::CubeFace, glm::vec3> RubicsCube::NORMALS_OF_FACES = {
	   {CubeFace::RIGHT_FACE,	glm::vec3(1.0f, 0.0f, 0.0f)},
	   {CubeFace::TOP_FACE,		glm::vec3(0.0f, 1.0f, 0.0f)},
	   {CubeFace::FRONT_FACE,	glm::vec3(0.0f, 0.0f, 1.0f)},
	   {CubeFace::LEFT_FACE,	glm::vec3(-1.0f, 0.0f, 0.0f)},
	   {CubeFace::BOTTOM_FACE,	glm::vec3(0.0f, -1.0f, 0.0f)},
	   {CubeFace::BACK_FACE,	glm::vec3(0.0f, 0.0f, -1.0f)}
};

const std::map<RubicsCube::CubeSlice, glm::vec3> RubicsCube::NORMALS_OF_SLICES = {
	   {CubeSlice::RIGHT_SLICE,	glm::vec3(1.0f, 0.0f, 0.0f)},
	   {CubeSlice::TOP_SLICE,	glm::vec3(0.0f, 1.0f, 0.0f)},
	   {CubeSlice::FRONT_SLICE,	glm::vec3(0.0f, 0.0f, 1.0f)},
	   {CubeSlice::LEFT_SLICE,	glm::vec3(1.0f, 0.0f, 0.0f)},
	   {CubeSlice::BOTTOM_SLICE,glm::vec3(0.0f, 1.0f, 0.0f)},
	   {CubeSlice::BACK_SLICE,	glm::vec3(0.0f, 0.0f, 1.0f)},
	   {CubeSlice::X_MID_SLICE,	glm::vec3(1.0f, 0.0f, 0.0f)},
	   {CubeSlice::Y_MID_SLICE,	glm::vec3(0.0f, 1.0f, 0.0f)},
	   {CubeSlice::Z_MID_SLICE,	glm::vec3(0.0f, 0.0f, 1.0f)},
};