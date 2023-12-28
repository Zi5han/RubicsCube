#pragma once
#include "CubieRenderer.h"

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
};