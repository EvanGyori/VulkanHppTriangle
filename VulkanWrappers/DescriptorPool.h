#pragma once

#include <vulkan/vulkan_raii.hpp>

class DescriptorPool
{
public:
    DescriptorPool(
	vk::raii::Device& device,
	vk::raii::DescriptorSetLayout& descriptorSetLayout,
	vk::raii::ImageView& imageView);

    vk::raii::DescriptorSet& getDescriptorSetHandle();

private:
    vk::raii::DescriptorPool handle;
    vk::raii::DescriptorSet descriptorSet;
};
