#pragma once
#include "AbstractGameInterface.h"
#include "CubieRenderer.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class GameInterface : public AbstractGameInterface
{
public:
	virtual void Initialize() override;
	virtual void RenderInterface(float aspectRatio) override;
	virtual void Update(double deltaTime) override;
	virtual void ClearResources() override;

	void RecalculateMatrices(float aspectRatio);

private:
	CubieRenderer m_cubieRenderer;
	float m_turningAngle;

	//MATRICES
	glm::mat4 m_projection;
	glm::mat4 m_view;
	glm::mat4 m_model;
};