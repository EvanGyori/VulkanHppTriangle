#include "Config.h"
#include "HostMappedBuffer.h"

HostMappedBuffer::HostMappedBuffer(
    Device& device,
    vk::DeviceSize size,
    vk::BufferUsageFlags usage)
    : Buffer(device, size, usage,
	vk::MemoryPropertyFlagBits::eHostVisible,
	vk::MemoryPropertyFlagBits::eDeviceLocal),
    data(nullptr)
{
    data = getMemory().mapMemory(0, VK_WHOLE_SIZE, {});
}

void* HostMappedBuffer::getData()
{
    return data;
}
