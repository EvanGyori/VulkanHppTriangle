#include "Config.h"
#include "VulkanUtility.h"

#include <stdexcept>

uint32_t findMemoryTypeWithProperties(
    vk::raii::PhysicalDevice& physicalDevice,
    uint32_t memoryTypeBitsRequirements,
    vk::MemoryPropertyFlags memoryPropertyRequirements)
{
    vk::PhysicalDeviceMemoryProperties memoryProperties =
	physicalDevice.getMemoryProperties();
    
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
	bool isRequiredMemoryType = memoryTypeBitsRequirements & (1 << i);
	bool hasRequiredProperties = (memoryProperties.memoryTypes[i].propertyFlags &
	    memoryPropertyRequirements) == memoryPropertyRequirements;

	if (isRequiredMemoryType && hasRequiredProperties) {
	    return i;
	}
    }

    throw std::runtime_error("VULKAN failed to find required memory type with required properties");
    return 0;
}
