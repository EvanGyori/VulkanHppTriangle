#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "QueueFamily.h"

class Device
{
public:
    Device(vk::raii::Instance& instance, vk::raii::SurfaceKHR& surface);

    vk::raii::Queue& getGraphicsQueue();

    vk::raii::Queue& getPresentQueue();

    uint32_t getGraphicsQueueFamilyIndex();

    uint32_t getPresentQueueFamilyIndex();

    vk::raii::PhysicalDevice& getPhysicalDevice();

    vk::raii::Device& getHandle();

private:
    vk::raii::PhysicalDevice physicalDevice;
    vk::raii::Device handle;
    std::vector<QueueFamily> queueFamilies;
};
