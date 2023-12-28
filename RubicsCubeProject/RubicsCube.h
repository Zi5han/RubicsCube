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

	RubicsCube() {};
	void Initialize();
	void Render(const glm::mat4& viewProjection);
	void Update(float deltaTime);
	void ClearResources();

private:
	CubieRenderer m_cubieRenderer;
};