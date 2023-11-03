#pragma once
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <vector>
#include "OBJModel.h"

class ObjCubieRenderer {
public:
	void Initialize();
	void Render(const glm::mat4& transformationMatrix);
	void ClearResources();

	float GetCubieExtention();

private:
	const float m_offset = 1.0f;

	OBJModel m_Model;

	void TranscribeVec3ToFloatArray(const std::vector<glm::vec3>& vecArray, const int arraySize, float* floatArray);

	GLuint m_arrayBufferObject;
	GLuint m_vertexBufferObject[2];
	GLuint m_shaderProgram;
	GLuint m_transformLocation;
};