#pragma once
#include "AbstractGameInterface.h"
#include <GL/glew.h>

class TestTriangle : public AbstractGameInterface {
public:
	virtual void Initialize();
	virtual void RenderInterface(float aspectRatio);
	virtual void ClearResources();
	virtual void Update(double deltaTime);

private:
	GLuint m_shaderProgram;
	GLuint m_vertexBufferObject;
	GLuint m_arrayBufferObject;
	GLint m_transformLocation;

	float m_turningAngle;
};