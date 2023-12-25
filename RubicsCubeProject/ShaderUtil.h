#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

class ShaderUtil {
public:
	static GLuint CreateShaderProgram(const char* vertexFilename, const char* fragmentFilename);

private:
	static std::string LoadFile(const char* fileName);
	static void PrintShaderLog(GLuint shader);
	static void PrintProgramLog(GLuint program);
	static void PrintOGlErrors();
};
