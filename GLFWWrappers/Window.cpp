#include "Config.h"
#include "Window.h"

#include <stdexcept>

static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

Window::Window(const char* title, int width, int height)
{
    GLFWManager::init();

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    handle = glfwCreateWindow(mode->width, mode->height, title, glfwGetPrimaryMonitor(), nullptr);
    if (handle == nullptr) {
	throw std::runtime_error("GLFW failed to create a window");
    }

    glfwSetWindowUserPointer(handle, this);
    glfwSetFramebufferSizeCallback(handle, framebufferSizeCallback);
}

Window::~Window()
{
    glfwDestroyWindow(handle);
}

GLFWwindow* Window::getHandle()
{
    return handle;
}

void Window::subscribeToFramebufferSizeEvent(FramebufferSizeCallbackFunc callback)
{
    framebufferSizeSubscribers.push_back(callback);
}

void Window::signalFramebufferSizeSubscribers(int width, int height)
{
    for (auto subscriber : framebufferSizeSubscribers) {
	subscriber(width, height);
    }
}

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Window* userPtr = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    userPtr->signalFramebufferSizeSubscribers(width, height);
}
