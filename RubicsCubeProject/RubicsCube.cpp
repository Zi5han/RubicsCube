
#include "GameInterface.h"
#include "TestGlm.h"
#define GLEW_STATIC
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

TestGlm gGlmTest;
GameInterface gDummyTest;

GameInterface* gUsedInterface;

GLFWwindow* InitializeSystem() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "Rubics Cube", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    gUsedInterface->Initialize(window);

    return window;
}

void RunCoreLoop(GLFWwindow* window) {
    double lastTime = glfwGetTime();
    double timeDiffrence = 0.0;

    float bg_red = 0.3;
    float bg_green = 0.6f;
    float bg_blue = 1.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        gUsedInterface->Update(timeDiffrence);
        //gUsedInterface->Update(0.1); // fürs Debuggen
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

        glViewport(0, 0, screenWidth, screenHeight);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(bg_red, bg_green, bg_blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        gUsedInterface->Render(aspectRatio);
        glfwSwapBuffers(window);


        bg_red = (sin(10 * lastTime + (3.14f/3)) / 4.0f) + 0.75f;
        bg_green = (sin(10 * lastTime + 2 * (3.14f / 3)) / 4.0f) + 0.75f;
        bg_blue = (sin(10 * lastTime) / 4.0f) + 0.75f;
       
        double currenTime = glfwGetTime();
        timeDiffrence = currenTime - lastTime;
        lastTime = currenTime;
    }
}

void ShutdownSystem() {
    gUsedInterface->ClearResources();
    glfwTerminate();

}

int main()
{
    //gUsedInterface = &gDummyTest;
    gUsedInterface = &gGlmTest;
    GLFWwindow* window = InitializeSystem();
    RunCoreLoop(window);
    ShutdownSystem();
}