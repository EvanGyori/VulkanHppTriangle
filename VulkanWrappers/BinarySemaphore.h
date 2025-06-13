#pragma once

#include <vulkan/vulkan_raii.hpp>

class BinarySemaphore
{
public:
    BinarySemaphore(vk::raii::Device& device);

    vk::raii::Semaphore& getHandle();

private:
    vk::raii::Semaphore handle;

};
