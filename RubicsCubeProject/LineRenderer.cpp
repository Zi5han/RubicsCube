
#include "LineRenderer.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>

void LineRenderer::Initialize() {


	m_shaderProgram = ShaderUtil::CreateShaderProgram("VLine.glsl", "FLine.glsl");
	m_transformLocation = glGetUniformLocation(m_shaderProgram, "transformation");
	m_colorLocation = glGetUniformLocation(m_shaderProgram, "color");
}

void LineRenderer::Render3D(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model, const glm::vec3& startPoint, const glm::vec3& endPoint, const glm::vec3& color) {
	glm::mat4 globalTransformation = projection * view * model;

	std::vector<float> vertices = {
	startPoint.x, startPoint.y, startPoint.z,
	endPoint.x, endPoint.y, endPoint.z
	};

	glGenVertexArrays(1, &m_arrayBufferObject);
	glGenBuffers(1, &m_vertexBufferObject);

	glBindVertexArray(m_arrayBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glUseProgram(m_shaderProgram);

	glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(globalTransformation));
	glUniform3fv(m_colorLocation, 1, glm::value_ptr(color));

	glLineWidth(4.0f);
	glDrawArrays(GL_LINES, 0, 2);

	glBindVertexArray(0);
	glUseProgram(0);
}

void LineRenderer::Render2D(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model, const glm::vec2& startPoint, const glm::vec2& endPoint, const glm::vec3& color) {
	// Konvertiere die 2D-Koordinaten in 3D-Koordinaten (z = 0)
	glm::vec3 startPoint3D(startPoint.x, startPoint.y, 0.0f);
	glm::vec3 endPoint3D(endPoint.x, endPoint.y, 0.0f);

	Render3D(glm::mat4(1.0f), glm::mat4(1.0f), model, startPoint3D, endPoint3D, color);
}

void LineRenderer::ClearResources() {
	glDeleteBuffers(1, &m_vertexBufferObject);
	glDeleteVertexArrays(1, &m_arrayBufferObject);
	glDeleteProgram(m_shaderProgram);
	glDeleteProgram(m_2DLineShaderProgram);
}
