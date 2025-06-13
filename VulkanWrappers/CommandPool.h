#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "Device.h"

class CommandPool
{
public:
    CommandPool(
	vk::raii::Device& device,
	uint32_t queueFamily);

    vk::raii::CommandPool& getHandle();

private:
    vk::raii::CommandPool handle;

};
