#pragma once
#include "GameInterface.h"
#include "ObjCubieRenderer.h"

class TestCompoundCube : public GameInterface
{
public:
	virtual void Initialize();
	virtual void Render(float aspectRatio);
	virtual void ClearResources();
	virtual void Update(double deltaTime);

private:
	ObjCubieRenderer m_cubieRenderer;
	float m_turningAngle;
};