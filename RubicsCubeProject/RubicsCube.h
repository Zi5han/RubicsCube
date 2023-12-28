#pragma once
#include "CubieRenderer.h"
#include "LineRenderer.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <glm/ext.hpp>

class GameInterface;
class CubieRenderer;
class Cubie3DGrid;

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

	glm::quat m_modelRotation;
	glm::vec2 m_previousScreenPosition;

	Face m_clickedFace = UNSET;

	const std::map<Face, glm::vec3> NORMALS_OF_FACES = {
	   {RIGHT, glm::vec3(1.0f, 0.0f, 0.0f)},
	   {TOP, glm::vec3(0.0f, 1.0f, 0.0f)},
	   {FRONT, glm::vec3(0.0f, 0.0f, 1.0f)},
	   {LEFT, glm::vec3(-1.0f, 0.0f, 0.0f)},
	   {BOTTOM, glm::vec3(0.0f, -1.0f, 0.0f)},
	   {BACK, glm::vec3(0.0f, 0.0f, -1.0f)}
	};

	//DEGUB
	LineRenderer d_lr;
	glm::vec3 d_objectSpaceFaceNormal;

	const GameInterface* d_gameInterface;
	glm::vec3 d_endPoint;
};