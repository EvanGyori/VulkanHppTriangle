#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class GLFWManager
{
public:
    static void init();

private:
    static GLFWManager* instance;

    GLFWManager() = default;

    ~GLFWManager();
};
