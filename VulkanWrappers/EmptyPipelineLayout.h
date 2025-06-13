#pragma once

#include <vulkan/vulkan_raii.hpp>

class EmptyPipelineLayout
{
public:
    EmptyPipelineLayout(vk::raii::Device& device);

    vk::raii::PipelineLayout& getHandle();

private:
    vk::raii::PipelineLayout handle;

};
