#pragma once

#include <vulkan/vulkan_raii.hpp>

class RenderPass
{
public:
    RenderPass(vk::raii::Device& device);

    vk::raii::RenderPass& getHandle();

private:
    vk::raii::RenderPass handle;

};
