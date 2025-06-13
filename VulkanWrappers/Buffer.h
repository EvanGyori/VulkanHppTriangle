#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "DeviceMemory.h"

class Buffer
{
public:
    Buffer(
	Device& device,
	vk::DeviceSize size,
	vk::BufferUsageFlags usage,
	vk::MemoryPropertyFlags requiredProperties = {},
	vk::MemoryPropertyFlags optimalProperties = {});

    vk::raii::Buffer& getHandle();

    vk::raii::DeviceMemory& getMemory();

private:
    vk::raii::Buffer handle;
    DeviceMemory memory;

};
