#include "Config.h"
#include "DeviceMemory.h"

#include <stdexcept>

#include "VulkanUtility.h"

DeviceMemory::DeviceMemory(nullptr_t) :
    handle(nullptr)
{
}

DeviceMemory::DeviceMemory(
    Device& device,
    vk::DeviceSize size,
    uint32_t requiredMemoryTypesBitmask,
    vk::MemoryPropertyFlags requiredProperties,
    vk::MemoryPropertyFlags optimalProperties) 
    : handle(nullptr)
{
    uint32_t memoryTypeIndex;
    try {
	memoryTypeIndex = findMemoryTypeWithProperties(
	    device.getPhysicalDevice(), requiredMemoryTypesBitmask,
	    requiredProperties | optimalProperties);
    } catch (std::runtime_error& err) {
	// failed to get optimal memory type
	memoryTypeIndex = findMemoryTypeWithProperties(
	    device.getPhysicalDevice(), requiredMemoryTypesBitmask, requiredProperties);
    }

    vk::MemoryAllocateInfo allocInfo(size, memoryTypeIndex);
    handle = device.getHandle().allocateMemory(allocInfo);
}

vk::raii::DeviceMemory& DeviceMemory::getHandle()
{
    return handle;
}
