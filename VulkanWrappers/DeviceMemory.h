#pragma once

#include <cstddef>

#include <vulkan/vulkan_raii.hpp>

#include "Device.h"

class DeviceMemory
{
public:
    DeviceMemory(nullptr_t);
    
    // Allocates some device memory with a memory type that atleast has the requiredProperties,
    // but also has the optimalProperties if a memory type was found with it
    DeviceMemory(
	Device& device,
	vk::DeviceSize size,
	uint32_t requiredMemoryTypesBitmask,
	vk::MemoryPropertyFlags requiredProperties,
	vk::MemoryPropertyFlags optimalProperties = {});

    vk::raii::DeviceMemory& getHandle();

private:
    vk::raii::DeviceMemory handle;
};
