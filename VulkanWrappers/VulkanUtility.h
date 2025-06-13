#pragma once

#include <vulkan/vulkan_raii.hpp>

// memoryTypeBitsRequirement is a bitmask specifying which memory types can be used
// Throws an error if a memory type couldn't be found
uint32_t findMemoryTypeWithProperties(
    vk::raii::PhysicalDevice& physicalDevice,
    uint32_t memoryTypeBitsRequirements,
    vk::MemoryPropertyFlags memoryPropertyRequirements);
