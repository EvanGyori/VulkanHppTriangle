#pragma once

#include <vulkan/vulkan_raii.hpp>

class PipelineLayout
{
public:
    PipelineLayout(
	vk::raii::Device& device,
	vk::DescriptorSetLayout descriptorSetLayout);

    vk::raii::PipelineLayout& getHandle();

private:
    vk::raii::PipelineLayout handle;
};
