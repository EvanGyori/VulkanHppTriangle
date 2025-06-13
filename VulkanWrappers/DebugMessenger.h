#pragma once

#include <vulkan/vulkan_raii.hpp>

class DebugMessenger
{
public:
    DebugMessenger(vk::raii::Instance& instance);

    static vk::DebugUtilsMessengerCreateInfoEXT getCreateInfo();

private:
    vk::raii::DebugUtilsMessengerEXT handle;
};
