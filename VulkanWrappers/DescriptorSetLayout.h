#pragma once

#include <vulkan/vulkan_raii.hpp>

class DescriptorSetLayout
{
public:
    DescriptorSetLayout(
	vk::raii::Device& device,
	vk::Sampler sampler);

    vk::raii::DescriptorSetLayout& getHandle();

private:
    vk::raii::DescriptorSetLayout handle;
};
