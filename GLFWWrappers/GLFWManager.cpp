#include "Config.h"
#include "GLFWManager.h"

#include <stdexcept>
#include <iostream>

#ifndef NDEBUG
static void glfwErrorCallback(int errorCode, const char* description);
#endif

GLFWManager* GLFWManager::instance = nullptr;

void GLFWManager::init()
{
    if (instance == nullptr) {
#ifndef NDEBUG
	glfwSetErrorCallback(glfwErrorCallback);
#endif

	if (glfwInit() == GLFW_FALSE) {
	    throw std::runtime_error("GLFW failed to init");
	}
	instance = new GLFWManager();
    }
}

GLFWManager::~GLFWManager()
{
    glfwTerminate();
}

#ifndef NDEBUG
static void glfwErrorCallback(int errorCode, const char* description)
{
    std::cerr << "GLFW ERROR " << errorCode << ": " << description << std::endl;
}
#endif
