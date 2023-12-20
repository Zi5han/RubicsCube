#pragma once
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <vector>

class LineRenderer {
public:
	void Initialize();
	void Render3D(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model,
		const glm::vec3& startPoint, const glm::vec3& endPoint, const glm::vec3& color = glm::vec3(0.0f));
	void Render2D(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model,
		const glm::vec2& startPoint, const glm::vec2& endPoint, const glm::vec3& color = glm::vec3(0.0f));
	void ClearResources();

private:
	GLuint m_arrayBufferObject;
	GLuint m_vertexBufferObject;
	GLuint m_shaderProgram;
	GLuint m_transformLocation;
	GLuint m_colorLocation;

	GLuint m_2DLineShaderProgram;
	GLuint m_2DLineTransformLocation;
	GLuint m_2DLineColorLocation;
};
