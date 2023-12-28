#pragma once
#include "CubieRenderer.h"
#include "LineRenderer.h"
#include "InputSystem.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <glm/ext.hpp>

class GameInterface;

class RubicsCube {
public:
	enum State {
		STABLE = 0,
		ROTATING = 1,
		SNAPING = 2
	};

	enum Axis {
		X = 0,
		Y = 1,
		Z = 2
	};

	enum Face {
		UNSET = -1,
		FRONT = 0,
		RIGHT = 1,
		TOP = 2,
		BACK = 3,
		LEFT = 4,
		BOTTOM = 5
	};
	void Initialize();
	void Render(const glm::mat4& viewProjection);
	void Update(const GameInterface& gameInterface);
	void ClearResources();

private:
	CubieRenderer m_cubieRenderer;
	const InputSystem* m_inputSystem;

	glm::quat m_modelRotation;
	glm::vec2 m_previousScreenPosition;

	Face m_clickedFace = UNSET;


	//ANIMATION
	State m_animationState = STABLE;

	//HELPING METHODS
	void h_RotateCube(const GameInterface& gameInterface);
	void h_DetermineClickedFace(const GameInterface& gameInterface);

	//STATIC
	const static std::map<RubicsCube::Face, glm::vec3> NORMALS_OF_FACES;

	//DEGUB
	LineRenderer d_lr;
	glm::vec3 d_objectSpaceFaceNormal;

	const GameInterface* d_gameInterface;
	glm::vec3 d_endPoint;
};