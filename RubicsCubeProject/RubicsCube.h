#pragma once
#include "CubieRenderer.h"
#include "LineRenderer.h"
#include "InputSystem.h"
#include "Cubie.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <array>

#include <glm/ext.hpp>

class GameInterface;

class RubicsCube {
public:
	enum AnimationState {
		STABLE = 0,
		ROTATING = 1,
		SNAPING = 2
	};

	enum Axis {
		UNSET_AXIS = -1,
		X = 0,
		Y = 1,
		Z = 2
	};

	enum CubeFace {
		UNSET_FACE = -1,
		RIGHT_FACE = 0,
		TOP_FACE = 1,
		FRONT_FACE = 2,
		LEFT_FACE = 3,
		BOTTOM_FACE = 4,
		BACK_FACE = 5
	};

	enum CubeSlice {
		UNSET_SLICE = -1,
		RIGHT_SLICE = 0,
		TOP_SLICE = 1,
		FRONT_SLICE = 2,
		LEFT_SLICE = 3,
		BOTTOM_SLICE = 4,
		BACK_SLICE = 5,
		X_MID_SLICE = 6,
		Y_MID_SLICE = 7,
		Z_MID_SLICE = 8
	};

	void Initialize(const GameInterface& gameInterface);
	void Render(const glm::mat4& viewProjection);
	void Update(const GameInterface& gameInterface);
	void ClearResources();

private:
	//COMPONENTS
	CubieRenderer m_cubieRenderer;
	const InputSystem* m_inputSystem;

	//CACHE
	glm::vec2 m_previousScreenPosition;

	//CUBE
	glm::quat m_modelRotation;
	std::array <std::array <std::array <Cubie*, 3>, 3>, 3> m_grid;
	void h_RotateCube();

	//FACE ROTATION
	CubeFace m_clickedFace = UNSET_FACE;

	Axis m_activeFaceNormal = UNSET_AXIS;
	int m_xSlice = 0;
	int m_ySlice = 0;
	int m_zSlice = 0;

	bool isActiveFaceSet = false;
	std::array<std::array<Cubie*, 3>, 3> m_activeFaceSliceArray;
	glm::vec3 m_facePlaneIntersectionPoint;
	void h_RotateFace();
	void h_DetermineClickedFace();
	void h_DetermineActiveFace();
	glm::vec3 h_findClosestDirection(const glm::vec3& referenceDirection, const glm::vec3& vectorU, const glm::vec3& vectorV);


	//ANIMATION
	AnimationState m_animationState = STABLE;
	void h_UpdateAnimation();

	//INPUT 
	void h_UpdateMouse();
	void h_UpdateKeyInput();

	//STATIC
	const static std::map<RubicsCube::CubeFace, glm::vec3> NORMALS_OF_FACES;
	const static std::map<RubicsCube::CubeSlice, glm::vec3> NORMALS_OF_SLICES;

	//DEGUB
	LineRenderer d_lr;
	glm::vec3 d_objectSpaceFaceNormal;

	const GameInterface* d_gameInterface;
	glm::vec3 d_endPoint;
};