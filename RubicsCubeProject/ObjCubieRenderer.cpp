#include "ObjCubieRenderer.h"
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void ObjCubieRenderer::Initialize() {
	m_Model = OBJModel("Cube.obj");

	const std::vector<Triangle> faces = m_Model.GetTrianglesData();

	// Amount of faces * amount of vertices per face * amount of coordinates per vertex
	int arraySize = faces.size() * 3 * 3;
	float* floatArray = new float[arraySize];

	ColorValue tempColor;

	std::vector<glm::vec3> positionField;
	std::vector<glm::vec3> colorField;
	std::vector<glm::vec2> uvField;
	std::vector<glm::vec3> normalField;

	for (const auto& face : faces) {
		tempColor = m_Model.getColorValue(face.material_name);
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
	TranscribeVec3ToFloatArray(positionField, arraySize, floatArray);
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(float), floatArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[1]);
	TranscribeVec3ToFloatArray(colorField, arraySize, floatArray);
	glBufferData(GL_ARRAY_BUFFER, arraySize * sizeof(float), floatArray, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	//// UV
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[2]);
	//TranscribeVec3ToFloatArray(uvField, arraySize, floatArray);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(floatArray), floatArray, GL_STATIC_DRAW);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(2);

	//// Normal
	//glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[3]);
	//TranscribeVec3ToFloatArray(normalField, arraySize, floatArray);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(floatArray), floatArray, GL_STATIC_DRAW);
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] floatArray;
}

void ObjCubieRenderer::Render(const glm::mat4& transformationMatrix) {
	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_arrayBufferObject);

	glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(transformationMatrix));
	glDrawArrays(GL_TRIANGLES, 0, m_Model.getVertexAmount());

	glBindVertexArray(0);
	glUseProgram(0);
}

void ObjCubieRenderer::ClearResources() {
	glDeleteBuffers(2, m_vertexBufferObject);
	glDeleteVertexArrays(1, &m_arrayBufferObject);
	glDeleteProgram(m_shaderProgram);
}

float ObjCubieRenderer::GetCubieExtenstions() {
	return 0.0f;
}

void ObjCubieRenderer::TranscribeVec3ToFloatArray(const std::vector<glm::vec3>& vecArray, const int arraySize, float* floatArray) {
	int writingCounter = 0;
	for (int i = 0; i < arraySize / 3; i++) {
		for (int coord = 0; coord < 3; coord++) {
			floatArray[writingCounter++] = vecArray[i][coord];
		}
	}
}
