#pragma once

struct GLFWwindow;

class AbstractGameInterface {
public:
	virtual void Initialize() {}
	virtual void Initialize(GLFWwindow* window) { Initialize(); }
	virtual void RenderInterface(float aspectRatio) {}
	virtual void Update(double deltaTime) {}
	virtual void ClearResources() {}
};