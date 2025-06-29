#include "Config.h"
#include "DescriptorPool.h"

#include <utility>

DescriptorPool::DescriptorPool(
    vk::raii::Device& device,
    vk::raii::DescriptorSetLayout& descriptorSetLayout,
    vk::raii::ImageView& imageView)
    : handle(nullptr),
    descriptorSet(nullptr)
{
    // Create descriptor pool
    vk::DescriptorPoolSize poolSize(vk::DescriptorType::eCombinedImageSampler, 1);
    vk::DescriptorPoolCreateInfo createInfo({}, 1, poolSize);
    handle = device.createDescriptorPool(createInfo);

    // Allocate descriptor set from pool
    vk::DescriptorSetLayout setLayoutHandle = descriptorSetLayout;
    vk::DescriptorSetAllocateInfo allocInfo(handle, setLayoutHandle);
    std::vector<vk::raii::DescriptorSet> sets = device.allocateDescriptorSets(allocInfo);
    descriptorSet = std::move(sets[0]);

    // write descriptor set
    vk::DescriptorImageInfo imageInfo({}, imageView, vk::ImageLayout::eShaderReadOnlyOptimal);
    vk::WriteDescriptorSet writeInfo(descriptorSet, 0, 0,
	vk::DescriptorType::eCombinedImageSampler, imageInfo);
    device.updateDescriptorSets(writeInfo, {});
}

vk::raii::DescriptorSet& DescriptorPool::getDescriptorSetHandle()
{
    return descriptorSet;
}
