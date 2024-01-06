#pragma once
#include "CubieRenderer.h"
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
	enum class AnimationState {
		STABLE = 0,
		ROTATING = 1,
		SNAPING = 2
	};

	enum class Axis {
		UNSET_AXIS = -1,
		X = 0,
		Y = 1,
		Z = 2
	};

	enum class CubeFace {
		UNSET_FACE = -1,
		RIGHT_FACE = 0,
		TOP_FACE = 1,
		FRONT_FACE = 2,
		LEFT_FACE = 3,
		BOTTOM_FACE = 4,
		BACK_FACE = 5
	};

	void Initialize(const GameInterface& gameInterface);
	void Render(const glm::mat4& viewProjection);
	void Update(const GameInterface& gameInterface);
	void ClearResources();

private:
	//INPUT 
	void i_UpdateMouse();

	//CUBE
	void c_RotateCube();

	//FACE ROTATION
	void fr_DetermineClickedFace();
	void fr_DetermineActiveFace();
	glm::vec3 fr_findClosestDirection(const glm::vec3& referenceDirection, const glm::vec3& vectorU, const glm::vec3& vectorV);
	void fr_DeltaRotateFace();

	//ANIMATION
	void a_StartSnappingAnimation();
	void a_UpdateAnimation(float deltaTime);

	//OTHER HELPING METHODS
	template<typename Func>
	void h_ForEachInSlice(Func func);

private:
	//COMPONENTS
	CubieRenderer m_cubieRenderer;
	const InputSystem* m_input;

	//CUBE
	glm::quat m_modelRotation;
	std::array <std::array <std::array <Cubie*, 3>, 3>, 3> m_grid;

	//FACE ROTATION
	CubeFace m_fr_clickedFace = CubeFace::UNSET_FACE;

	Axis m_fr_activeFaceNormal = Axis::UNSET_AXIS;
	int m_fr_xSliceIndex = 0;
	int m_fr_ySliceIndex = 0;
	int m_fr_zSliceIndex = 0;

	glm::vec3 m_fr_facePlaneIntersectionPoint;

	//ANIMATION
	AnimationState m_a_animationState = AnimationState::STABLE;
	float m_a_totalFaceRotationDegree;
	std::array<std::array<glm::quat, 3>, 3> m_a_oldVisibleRotations;
	float m_a_tickCounter;

	//CACHE
	glm::vec2 m_previousScreenPosition;

	//STATIC
	const static std::map<int, glm::vec3> NORMALS_OF_FACES;
};