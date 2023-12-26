#pragma once
#include "AbstractGameInterface.h"
#include "TestCompoundCube.h"
#include "SimpleCubieRenderer.h"

class TestCubie : public AbstractGameInterface {
public:
	virtual void Initialize();
	virtual void RenderInterface(float aspectRatio);
	virtual void ClearResources();
	virtual void Update(double deltaTime);

private:
	CubieRenderer m_cubieRenderer;
	float m_turningAngle;
};
