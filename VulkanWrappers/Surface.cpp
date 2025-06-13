#include "Config.h"
#include "Surface.h"

#include <stdexcept>

#include "VulkanRaiiContext.h"

Surface::Surface(vk::raii::Instance& instance, GLFWwindow* window) :
    handle(nullptr)
{
    VkSurfaceKHR handleWithoutRaii;
    if (glfwCreateWindowSurface(*instance, window, nullptr, &handleWithoutRaii) != VK_SUCCESS) {
	throw std::runtime_error("VULKAN failed to create window surface");
    }

    handle = vk::raii::SurfaceKHR(instance, handleWithoutRaii);
}

vk::raii::SurfaceKHR& Surface::getHandle()
{
    return handle;
}
