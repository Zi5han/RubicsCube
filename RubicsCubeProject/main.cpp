#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "AbstractGameInterface.h"
#include "GameInterface.h"

#include <iostream>
#include <thread>


AbstractGameInterface gDummyTest;
GameInterface gameInterface;

AbstractGameInterface* gUsedInterface;

double lastTime = glfwGetTime();
double deltaTime = 0.0;

void RenderWindow(GLFWwindow* window) {
	gUsedInterface->Update(deltaTime);
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glDepthFunc(GL_LEQUAL);
	float bgColor[3] = { 28, 112, 230 };
	for (float& element : bgColor)
		element /= 256;
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int minimized = glfwGetWindowAttrib(window, GLFW_ICONIFIED);

	if (!minimized) {
		gUsedInterface->RenderInterface(aspectRatio);
	}
	glfwSwapBuffers(window);

	double currenTime = glfwGetTime();
	deltaTime = currenTime - lastTime;
	lastTime = currenTime;
	std::this_thread::sleep_for(std::chrono::milliseconds(8));
}

void RunCoreLoop(GLFWwindow* window) {

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		RenderWindow(window);
	}
}

GLFWwindow* InitializeSystem() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Rubics Cube", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetWindowRefreshCallback(window, RenderWindow);

	glewExperimental = true;
	glewInit();

	gUsedInterface->Initialize(window);

	return window;
}

void ShutdownSystem() {
	gUsedInterface->ClearResources();
	glfwTerminate();

}

int main() {
	gUsedInterface = &gameInterface;
	GLFWwindow* window = InitializeSystem();
	RunCoreLoop(window);
	ShutdownSystem();
}