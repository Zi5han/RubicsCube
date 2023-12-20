#pragma once

struct GLFWwindow;

class GameInterface {
public:
	virtual void Initialize() {}
	virtual void Initialize(GLFWwindow* window) { Initialize(); }
	virtual void RenderInterface(float aspectRation) {}
	virtual void Update(double deltaTime) {}
	virtual void ClearResources() {}
};