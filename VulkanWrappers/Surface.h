#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "GLFWManager.h"

class Surface
{
public:
    Surface(vk::raii::Instance& instance, GLFWwindow* window);

    vk::raii::SurfaceKHR& getHandle();

private:
    vk::raii::SurfaceKHR handle;
};
