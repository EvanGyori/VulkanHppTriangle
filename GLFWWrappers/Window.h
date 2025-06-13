#pragma once

#include <functional>
#include <vector>

#include "GLFWManager.h"

class Window
{
public:
    using FramebufferSizeCallbackFunc = std::function<void(int, int)>;

    Window(const char* title, int width, int height);

    ~Window();

    GLFWwindow* getHandle();

    void subscribeToFramebufferSizeEvent(FramebufferSizeCallbackFunc callback);

    // Called by GLFW callback when the framebuffer size changes for this window
    void signalFramebufferSizeSubscribers(int width, int height);

private:
    GLFWwindow* handle;

    std::vector<FramebufferSizeCallbackFunc> framebufferSizeSubscribers;
};
