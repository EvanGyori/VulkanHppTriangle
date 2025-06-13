#pragma once

#include <vector>

#include <vulkan/vulkan_raii.hpp>

struct QueueFamily
{
    // The queue family index as it appears in the queue family properties
    // array returned from the physical device
    uint32_t index;

    // Queues created for a logical device in the order of creation
    std::vector<vk::raii::Queue> queues;
};
