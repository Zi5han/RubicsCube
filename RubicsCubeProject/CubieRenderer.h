#pragma once
#include "ShaderUtil.h"

#include <glm/fwd.hpp>

class OBJModel;

class CubieRenderer {
public:
	CubieRenderer();
	void Initialize();
	void Render(const glm::mat4& projectionView, const glm::mat4& model);
	void ClearResources();

	float GetCubieExtention();

private:
	const float m_offset = 1.0f;

	OBJModel* m_cubeModel;
	GLuint m_arrayBufferObject;
	GLuint m_vertexBufferObject[2];
	GLuint m_shaderProgram;
	GLuint m_transformLocation;
};