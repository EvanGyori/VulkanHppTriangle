#include "Config.h"
#include "DescriptorSetLayout.h"

DescriptorSetLayout::DescriptorSetLayout(
    vk::raii::Device& device,
    vk::Sampler sampler)
    : handle(nullptr)
{
    vk::DescriptorSetLayoutBinding binding(0, vk::DescriptorType::eCombinedImageSampler,
	1, vk::ShaderStageFlagBits::eFragment, &sampler);

    vk::DescriptorSetLayoutCreateInfo createInfo({}, binding);

    handle = device.createDescriptorSetLayout(createInfo);
}

vk::raii::DescriptorSetLayout& DescriptorSetLayout::getHandle()
{
    return handle;
}
