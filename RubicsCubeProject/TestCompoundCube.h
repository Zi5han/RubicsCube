#pragma once
#include "GameInterface.h"
#include "CubieRenderer.h"

class TestCompoundCube : public GameInterface
{
public:
	virtual void Initialize() override;
	virtual void RenderInterface(float aspectRatio) override;
	virtual void Update(double deltaTime) override;
	virtual void ClearResources() override;

private:
	CubieRenderer m_cubieRenderer;
	float m_turningAngle;
};