#include "CubieRenderer.h"

#include "OBJModel.h"

#include <glm/gtc/type_ptr.hpp>

using namespace VertexStructs;

CubieRenderer::CubieRenderer() {
	m_arrayBufferObject = 0u;
	m_cubeModel = nullptr;
	m_shaderProgram = 0u;
	m_transformLocation = 0u;
	m_vertexBufferObject[0] = 0u;
	m_vertexBufferObject[1] = 0u;
}

void CubieRenderer::Initialize() {
	m_cubeModel = new OBJModel("Cube.obj");

	const std::vector<Triangle> faces = m_cubeModel->GetTrianglesData();

	ColorValue tempColor;

	std::vector<glm::vec3> positionField;
	std::vector<glm::vec3> colorField;
	std::vector<glm::vec2> uvField;
	std::vector<glm::vec3> normalField;

	for (const auto& face : faces) {
		tempColor = m_cubeModel->getColorValue(face.material_name);
		for (const auto& vertex : face.vetices) {
			positionField.push_back(glm::vec3{ vertex.position_x, vertex.position_y, vertex.position_z });
			colorField.push_back(glm::vec3{ tempColor.r, tempColor.g, tempColor.b });
			uvField.push_back(glm::vec2{ vertex.u, vertex.v });
			normalField.push_back(glm::vec3{ vertex.normal_x, vertex.normal_y, vertex.normal_z });
		}
	}

	m_shaderProgram = ShaderUtil::CreateShaderProgram("VColor.glsl", "FColor.glsl");
	m_transformLocation = glGetUniformLocation(m_shaderProgram, "transformation");

	glGenVertexArrays(1, &m_arrayBufferObject);
	glGenBuffers(2, m_vertexBufferObject);

	glBindVertexArray(m_arrayBufferObject);

	// Position
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positionField.size(), positionField.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colorField.size(), colorField.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	//// UV (here might be bugs, cuz UV has only 2 coords, while other fileds have 3)
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[2]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * uvField.size(), uvField.data(), GL_STATIC_DRAW);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(2);

	//// Normal
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[3]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * uvField.size(), uvField.data(), GL_STATIC_DRAW);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CubieRenderer::Render(const glm::mat4& projectionView, const glm::mat4& model) {
	glm::mat4 globalTransformation = projectionView * model;

	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_arrayBufferObject);

	glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(globalTransformation));
	glDrawArrays(GL_TRIANGLES, 0, m_cubeModel->getVertexAmount());

	glBindVertexArray(0);
	glUseProgram(0);
}

void CubieRenderer::ClearResources() {
	glDeleteBuffers(2, m_vertexBufferObject);
	glDeleteVertexArrays(1, &m_arrayBufferObject);
	glDeleteProgram(m_shaderProgram);
}

float CubieRenderer::GetCubieExtention() {
	//damit die Luecken zwischen den Minicubies erscheinen.
	return 2.0f * m_offset;
}