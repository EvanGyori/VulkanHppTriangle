#pragma once

#include <vulkan/vulkan_raii.hpp>

class Fence
{
public:
    Fence(vk::raii::Device& device, bool startSignaled);

    void wait();

    void reset();

    vk::raii::Fence& getHandle();

private:
    vk::raii::Device& device;
    vk::raii::Fence handle;

};
