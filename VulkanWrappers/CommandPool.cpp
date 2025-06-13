#include "CommandPool.h"

CommandPool::CommandPool(
    vk::raii::Device& device,
    uint32_t queueFamily)
    : handle(nullptr)
{
    vk::CommandPoolCreateInfo info(vk::CommandPoolCreateFlagBits::eTransient, queueFamily);
    handle = device.createCommandPool(info);
}

vk::raii::CommandPool& CommandPool::getHandle()
{
    return handle;
}
