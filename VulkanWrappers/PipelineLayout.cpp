#include "Config.h"
#include "PipelineLayout.h"

PipelineLayout::PipelineLayout(
    vk::raii::Device& device,
    vk::DescriptorSetLayout descriptorSetLayout)
    : handle(nullptr)
{
    vk::PipelineLayoutCreateInfo createInfo({}, descriptorSetLayout);

    handle = device.createPipelineLayout(createInfo);
}

vk::raii::PipelineLayout& PipelineLayout::getHandle()
{
    return handle;
}
