#include "RubicsCube.h"
#include "GameInterface.h"
#include <glm/gtx/intersect.hpp>
#include <iomanip>

void RubicsCube::Initialize(const GameInterface& gameInterface) {
	m_cubieRenderer.Initialize();
	d_lr.Initialize();

	float offset = m_cubieRenderer.GetCubieExtention();

	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			for (int z = 0; z < 3; z++) {
				m_grid[x][y][z] = new Cubie;
				m_grid[x][y][z]->m_number = (x * 3 * 3) + (y * 3) + z;
				m_grid[x][y][z]->m_position = glm::vec3((x - 1) * offset, (y - 1) * offset, (z - 1) * offset);
			}
	m_input = &gameInterface.GetInputComponent();
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
}

void RubicsCube::Update(const GameInterface& gameInterface) {
	//Animation
	if (m_a_animationState == AnimationState::SNAPING) {
		a_UpdateAnimation();
	}

	d_gameInterface = &gameInterface;

	i_UpdateMouse();
	i_UpdateKeyInput();

	m_previousScreenPosition = m_input->GetScreenPosition();
}

void RubicsCube::ClearResources() {
	m_cubieRenderer.ClearResources();
	for (auto& arr2d : m_grid)
		for (auto& arr : arr2d)
			for (auto& e : arr)
				delete e;
}

//INPUT
void RubicsCube::i_UpdateMouse() {

	//LEFT CLICK
	if (m_input->GetActiveMouseButton() == InputSystem::LEFT_BUTTON) {
		switch (m_input->GetLeftClickState()) {

		case InputSystem::CLICK:
			switch (m_a_animationState) {
			case AnimationState::STABLE:
				fr_DetermineClickedFace();
				break;
			case AnimationState::ROTATING:
				break;
			case AnimationState::SNAPING:
				break;
			default:
				break;
			}
			break;

		case InputSystem::HOLD:
			switch (m_a_animationState) {
			case AnimationState::STABLE:
				if (m_fr_clickedFace == CubeFace::UNSET_FACE)
					break;
				if (glm::length(m_input->GetScreenPosition() - m_input->GetDragStartScreenPosition()) > 10) {
					fr_DetermineActiveFace();
					m_a_animationState = AnimationState::ROTATING;
					break;
				}
				break;

			case AnimationState::ROTATING:
				if (m_fr_activeFaceNormal != Axis::UNSET_AXIS)
					fr_DeltaRotateFace();
				break;
			case AnimationState::SNAPING:
				break;
			default:
				break;
			}
			break;

		case InputSystem::RELEASE:
			if (m_a_animationState != AnimationState::STABLE) {
				//glm::mat4 rotationMatrix
				//	= glm::rotate(
				//		glm::mat4(1.0f),
				//		glm::radians(0.0f),
				//		NORMALS_OF_FACES.at(static_cast<int>(m_fr_activeFaceNormal))
				//	);
				//h_ForEachInSlice([&rotationMatrix](Cubie* cubie) { //BROKEN
				//	cubie->m_visibleRotation = rotationMatrix;
				//	});
				a_StartSnappingAnimation();
				m_a_animationState = AnimationState::SNAPING;
			}
			break;
		}
	}
	//RIGHT CLICK
	else if (m_input->GetActiveMouseButton() == InputSystem::RIGHT_BUTTON) {
		if (m_input->GetRightClickState() == InputSystem::HOLD) {
			c_RotateCube();
		}
	}
}

void RubicsCube::i_UpdateKeyInput() {
	if (m_input->IsKeyReleased(GLFW_KEY_SPACE))
		m_modelRotation = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
}

//CUBE
void RubicsCube::c_RotateCube() {
	//Calculate the difference between the current and previous screen positions
	glm::vec2 delta = m_input->GetScreenPosition() - m_previousScreenPosition;

	//Convert the delta vector to radians
	float angleX = glm::radians(delta.x);
	float angleY = glm::radians(delta.y);

	//Create quaternions for the rotations
	glm::quat rotationX = glm::angleAxis(angleX, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat rotationY = glm::angleAxis(angleY, glm::vec3(1.0f, 0.0f, 0.0f));

	//Apply the rotations to the cubeRotation
	m_modelRotation = rotationX * m_modelRotation;
	m_modelRotation = rotationY * m_modelRotation;
	m_modelRotation = glm::normalize(m_modelRotation);
}

//FACE ROTATION
void RubicsCube::fr_DetermineClickedFace() {
	glm::vec3 origin, direction;
	m_input->GetPickingRay(origin, direction);

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
		//0.1f is tolerance
		if ((intersectionPointInObjectSpace.x > (planeOffset + 0.1f) ||
			intersectionPointInObjectSpace.x < -(planeOffset + 0.1f) ||
			intersectionPointInObjectSpace.y >(planeOffset + 0.1f) ||
			intersectionPointInObjectSpace.y < -(planeOffset + 0.1f) ||
			intersectionPointInObjectSpace.z >(planeOffset + 0.1f) ||
			intersectionPointInObjectSpace.z < -(planeOffset + 0.1f)))
			continue;

		m_fr_clickedFace = static_cast<CubeFace>(normal.first);
		m_fr_facePlaneIntersectionPoint = intersectionPoint;
		//d_endPoint = intersectionPointInObjectSpace;
		//std::cout << m_fr_clickedFace << ' ' <<
		//	d_endPoint.x << ' ' <<
		//	d_endPoint.y << ' ' <<
		//	d_endPoint.z << '\n';
		break;
	}
}

void RubicsCube::fr_DetermineActiveFace() {
	//Es wird der Schnittpunkt in Object Space bestimmt
	glm::vec3 intersectionPointInObjectSpace
		= glm::inverse(glm::mat3_cast(m_modelRotation)) * m_fr_facePlaneIntersectionPoint;
	float planeOffset = 1.5f * m_cubieRenderer.GetCubieExtention();

	//Die slice indices werden bestimmt
	m_fr_xSliceIndex
		= (intersectionPointInObjectSpace.x >= (-planeOffset / 3)
			&& intersectionPointInObjectSpace.x <= (planeOffset / 3))
		? 1 : (intersectionPointInObjectSpace.x > (planeOffset / 3) ? 2 : 0);
	m_fr_ySliceIndex
		= (intersectionPointInObjectSpace.y >= (-planeOffset / 3)
			&& intersectionPointInObjectSpace.y <= (planeOffset / 3))
		? 1 : (intersectionPointInObjectSpace.y > (planeOffset / 3) ? 2 : 0);
	m_fr_zSliceIndex
		= (intersectionPointInObjectSpace.z >= (-planeOffset / 3)
			&& intersectionPointInObjectSpace.z <= (planeOffset / 3))
		? 1 : (intersectionPointInObjectSpace.z > (planeOffset / 3) ? 2 : 0);



	//Es wird der aktuelle Schnittpunkt auf der geklicken Ebene im Object Space bestimmt
	float intersectionDistance;
	glm::vec3 origin, direction;
	m_input->GetPickingRay(origin, direction);
	//Warnung: bei freier Kamerasteurung, könnte es auch kein Schnittpunkt geben
	//Hier wird die Normale in für die Rechnung rotiert, dannach wird die Rotation rückgängig gemacht
	glm::intersectRayPlane(origin,
		direction,
		glm::mat3_cast(m_modelRotation) * NORMALS_OF_FACES.at(static_cast<int>(m_fr_clickedFace)) * (planeOffset),
		glm::mat3_cast(m_modelRotation) * NORMALS_OF_FACES.at(static_cast<int>(m_fr_clickedFace)),
		intersectionDistance);
	glm::vec3 currenIntersectionPointInObjectSpace =
		glm::inverse(glm::mat3_cast(m_modelRotation)) * (origin + intersectionDistance * direction);

	//Normale der geklicken Face wird normalisiert
	int positiveNormalIndex = static_cast<int>(m_fr_clickedFace) % 3;

	//Die Drag Richtung wird bestimmt
	glm::vec3 dragDirection = currenIntersectionPointInObjectSpace - intersectionPointInObjectSpace;
	glm::vec3 dragDirectionNormal = fr_findClosestDirection(
		dragDirection,
		NORMALS_OF_FACES.at((positiveNormalIndex + 1) % 3),
		NORMALS_OF_FACES.at((positiveNormalIndex + 2) % 3));

	//d_endPoint = dragDirection * 10.0f;

	if (m_fr_clickedFace == CubeFace::LEFT_FACE || m_fr_clickedFace == CubeFace::RIGHT_FACE) {
		if (dragDirectionNormal.y == 1.0f) {
			m_fr_activeFaceNormal = Axis::Z;
		}
		else if (dragDirectionNormal.z == 1.0f) {
			m_fr_activeFaceNormal = Axis::Y;
		}
	}
	else if (m_fr_clickedFace == CubeFace::FRONT_FACE || m_fr_clickedFace == CubeFace::BACK_FACE) {
		if (dragDirectionNormal.x == 1.0f) {
			m_fr_activeFaceNormal = Axis::Y;
		}
		else if (dragDirectionNormal.y == 1.0f) {
			m_fr_activeFaceNormal = Axis::X;
		}
	}
	else { //m_fr_clickedFace == TOP_FACE || m_fr_clickedFace == BOTTOM_FACE
		if (dragDirectionNormal.x == 1.0f) {
			m_fr_activeFaceNormal = Axis::Z;
		}
		else if (dragDirectionNormal.z == 1.0f) {
			m_fr_activeFaceNormal = Axis::X;
		}
	}

	std::cout << "Der Slice ist "
		<< "X: " << m_fr_xSliceIndex << ", "
		<< "Y: " << m_fr_ySliceIndex << ", "
		<< "Z: " << m_fr_zSliceIndex << '\n';
	std::cout << "Die aktive Achse ist: " << static_cast<int>(m_fr_activeFaceNormal) << '\n';


	//std::cout << activeSliceX << ' ' << activeSliceY << ' ' << activeSliceZ << ' ' << '\n';
	//std::cout << intersectionPointInObjectSpace.x << ' ' << intersectionPointInObjectSpace.y << ' ' << intersectionPointInObjectSpace.z << ' ' << '\n';

}

void RubicsCube::fr_DeltaRotateFace() {
	glm::vec2 deltaDragVector = m_input->GetScreenPosition() - m_previousScreenPosition;

	int activeFaceIndex = static_cast<int>(m_fr_activeFaceNormal);
	//glm::vec3 activeFaceNormalInWorld
	//	= glm::mat3_cast(m_modelRotation) * NORMALS_OF_FACES.at(activeFaceIndex);
	int clickedFaceIndex = static_cast<int>(m_fr_clickedFace) % 3;
	//glm::vec3 clickedFaceNormalInWorld
	//	= glm::mat3_cast(m_modelRotation) * NORMALS_OF_FACES.at(clickedFaceIndex);

	//Index of the Normal that is not active nor clicked
	int dragFaceIndex = 3 - activeFaceIndex - clickedFaceIndex;
	glm::vec3 dragNormalInWorld
		= glm::mat3_cast(m_modelRotation) * NORMALS_OF_FACES.at(dragFaceIndex);

	glm::vec2 dragNormalInScreenSpace
		= m_input->WorldToScreen(dragNormalInWorld)
		- m_input->WorldToScreen(glm::vec3(0.0f));

	float scaleProjectedVector
		= glm::dot(deltaDragVector, dragNormalInScreenSpace) / glm::length(dragNormalInScreenSpace);

	float deltaRotation = glm::length(scaleProjectedVector * dragNormalInScreenSpace);

	if (scaleProjectedVector < 0)
		deltaRotation *= -1;

	//glm::vec3 localAxes
	//	= glm::mat3_cast(m_modelRotation) * NORMALS_OF_FACES.at(static_cast<int>(m_fr_activeFaceNormal));
	//
	//float localAxes_max = 0;
	//int localAxes_max_index = 0;
	//for (int i = 0; i < 3; i++) {
	//	if (localAxes[i] > localAxes_max) {
	//		localAxes_max = localAxes[i];
	//		localAxes_max_index = i;
	//	}
	//}
	//std::cout << "\r";
	//std::cout << std::fixed << std::setprecision(4) << std::setw(8) << localAxes_max_index; // Setzt die Breite jedes Elements auf 5
	//std::cout << "\n";
	//
	//
	//std::cout << "\r";
	//for (int i = 0; i < 3; ++i) {
	//	std::cout << std::fixed << std::setprecision(4) << std::setw(8) << localAxes[i]; // Setzt die Breite jedes Elements auf 5
	//	std::cout << "\n";
	//}
	//
	//std::cout << "\r";
	//for (int i = 0; i < 3; ++i) {
	//	for (int j = 0; j < 3; ++j) {
	//		std::cout << std::fixed << std::setprecision(4) << std::setw(8) << glm::mat3_cast(m_modelRotation)[i][j]; // Setzt die Breite jedes Elements auf 5
	//	}
	//	std::cout << "\n";
	//}

	// Nicht schön
	if (m_fr_clickedFace == CubeFace::RIGHT_FACE) {
		if (m_fr_activeFaceNormal == Axis::Y)
			deltaRotation *= -1;
	}
	else if (m_fr_clickedFace == CubeFace::TOP_FACE) {
		if (m_fr_activeFaceNormal == Axis::Z)
			deltaRotation *= -1;
	}
	else if (m_fr_clickedFace == CubeFace::FRONT_FACE)
	{
		if (m_fr_activeFaceNormal == Axis::X)
			deltaRotation *= -1;
	}
	else if (m_fr_clickedFace == CubeFace::LEFT_FACE) {
		if (m_fr_activeFaceNormal == Axis::Z)
			deltaRotation *= -1;
	}
	else if (m_fr_clickedFace == CubeFace::BOTTOM_FACE) {
		if (m_fr_activeFaceNormal == Axis::X)
			deltaRotation *= -1;
	}
	else if (m_fr_clickedFace == CubeFace::BACK_FACE) {
		if (m_fr_activeFaceNormal == Axis::Y)
			deltaRotation *= -1;
	}

	glm::mat4 rotationMatrix
		= glm::rotate(
			glm::mat4(1.0f),
			glm::radians(deltaRotation * 10.0f),
			NORMALS_OF_FACES.at(static_cast<int>(m_fr_activeFaceNormal))
		);
	h_ForEachInSlice([&rotationMatrix](Cubie* cubie, int index) {
		cubie->m_visibleRotation = cubie->m_visibleRotation * rotationMatrix;
		});
	m_a_totalFaceRotationDegree += deltaRotation * 10.0f;
}

glm::vec3 RubicsCube::fr_findClosestDirection(const glm::vec3& referenceDirection, const glm::vec3& vecU, const glm::vec3& vecV) {
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

	else //maxDotProduct == dotProductNegativeV 
		return vecV;

}

//ANIMATION

void RubicsCube::a_StartSnappingAnimation() {
	// Normalize total face rotation degree
	while (m_a_totalFaceRotationDegree > 360)
		m_a_totalFaceRotationDegree -= 360;
	while (m_a_totalFaceRotationDegree < 0)
		m_a_totalFaceRotationDegree += 360;

	if (m_a_totalFaceRotationDegree < 45.0f)
		m_a_totalFaceRotationDegree = 0.0f;
	else if (m_a_totalFaceRotationDegree < 135.0f)
		m_a_totalFaceRotationDegree = 90.0f;
	else if (m_a_totalFaceRotationDegree < 225.0f)
		m_a_totalFaceRotationDegree = 180.0f;
	else if (m_a_totalFaceRotationDegree < 315.0f)
		m_a_totalFaceRotationDegree = 270.0f;
	else
		m_a_totalFaceRotationDegree = 0.0f;

	glm::mat4 totalSnappedRotation
		= glm::rotate(glm::mat4(1.0f),
			glm::radians(m_a_totalFaceRotationDegree),
			NORMALS_OF_FACES.at(static_cast<int>(m_fr_activeFaceNormal) % 3));

	std::array<std::array<glm::mat4, 3>, 3> oldSnappedRotation;

	h_ForEachInSlice([&totalSnappedRotation, &oldSnappedRotation](Cubie* cubie, int index) {
		cubie->m_snapedRotation *= totalSnappedRotation;
		//cubie->m_visibleRotation = cubie->m_snapedRotation; //DEBUG
		oldSnappedRotation[static_cast<int>(index / 3)][index % 3] = cubie->m_visibleRotation; oldSnappedRotation[static_cast<int>(index / 3)][index % 3] = cubie->m_snapedRotation;
		}
	);

	m_a_totalFaceRotationDegree = 0;

	//DEBUG
	std::array<std::array<int, 3>, 3> numbers;
	h_ForEachInSlice([&numbers](Cubie* cubie, int index) {
		numbers[static_cast<int>(index / 3)][index % 3] = cubie->m_number;
		}
	);

	std::cout << "\r"; // Setzt den Cursor zurück zum Anfang der Zeile
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			std::cout << std::fixed << std::setprecision(4) << std::setw(8) << numbers[i][j]; // Setzt die Genauigkeit auf 4 Stellen nach dem Komma und die Breite jedes Elements auf 8
		}
		std::cout << "\n";
	}
}

void RubicsCube::a_UpdateAnimation() {
	m_a_animationState = AnimationState::STABLE;
}

//OTHER HELPING METHODS
// Erwartet void (Cubie* cubie, int index) 
template<typename Func>
void RubicsCube::h_ForEachInSlice(Func func) {
	switch (m_fr_activeFaceNormal) {
	case Axis::X:
		//
		//              Y
		//              ^
		//    [0][0]    |    [0][2]
		//          +---|---+
		//          |   |   |
		//  Z   <-------+   |
		//          |       |
		//          +-------+
		//    [2][0]         [2][2]
		//
		for (int i = 2; i >= 0; i--) {
			for (int j = 2; j >= 0; j--) {
				func(m_grid[m_fr_xSliceIndex][i][j], i * 3 + j);
			}
		}
		break;

	case Axis::Y:
		//
		//    [0][0]         [0][2]
		//          +-------+
		//          |       |
		//          |   +-------> X
		//          |   |   |
		//          +---|---+
		//    [2][0]    |    [2][2]
		//              V
		//              Z
		//
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				func(m_grid[i][m_fr_ySliceIndex][j], i * 3 + j);
			}
		}
		break;

	case Axis::Z:
		//
		//              Y
		//              ^
		//    [0][0]    |    [0][2]
		//          +---|---+
		//          |   |   |
		//          |   +-------> X
		//          |       |
		//          +-------+
		//    [2][0]         [2][2]
		//
		for (int i = 2; i >= 0; i--) {
			for (int j = 0; j < 3; ++j) {
				func(m_grid[i][j][m_fr_zSliceIndex], i * 3 + j);
			}
		}
		break;

	default:
		//ERROR
		break;
	}
}

//STATIC
const std::map<int, glm::vec3> RubicsCube::NORMALS_OF_FACES = {
	   {0,	glm::vec3(1.0f, 0.0f, 0.0f)},
	   {1,	glm::vec3(0.0f, 1.0f, 0.0f)},
	   {2,	glm::vec3(0.0f, 0.0f, 1.0f)},
	   {3,	glm::vec3(-1.0f, 0.0f, 0.0f)},
	   {4,	glm::vec3(0.0f, -1.0f, 0.0f)},
	   {5,	glm::vec3(0.0f, 0.0f, -1.0f)}
};