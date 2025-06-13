#include "Config.h"
#include "Buffer.h"

Buffer::Buffer(
    Device& device,
    vk::DeviceSize size,
    vk::BufferUsageFlags usage,
    vk::MemoryPropertyFlags requiredProperties,
    vk::MemoryPropertyFlags optimalProperties)
    : handle(nullptr),
    memory(nullptr)
{
    vk::BufferCreateInfo createInfo({}, size, usage, vk::SharingMode::eExclusive);
    handle = device.getHandle().createBuffer(createInfo);

    vk::MemoryRequirements requirements = handle.getMemoryRequirements();

    memory = DeviceMemory(device, requirements.size, requirements.memoryTypeBits,
	requiredProperties, optimalProperties);

    handle.bindMemory(memory.getHandle(), 0);
}

vk::raii::Buffer& Buffer::getHandle()
{
    return handle;
}

vk::raii::DeviceMemory& Buffer::getMemory()
{
    return memory.getHandle();
}
