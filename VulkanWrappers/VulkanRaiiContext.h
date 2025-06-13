#pragma once

#include <vulkan/vulkan_raii.hpp>

class VulkanRaiiContext
{
public:
    static vk::raii::Context& get();

private:
    static VulkanRaiiContext* instance;

    vk::raii::Context context;
};
