#include "Config.h"
#include "Window.h"

#include <stdexcept>

static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

Window::Window(const char* title, int width, int height)
{
    GLFWManager::init();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    handle = glfwCreateWindow(width, height, title, /* glfwGetPrimaryMonitor() */ nullptr, nullptr);
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
