#pragma once

#include <vulkan/vulkan_raii.hpp>

class Instance
{
public:
    Instance();

    vk::raii::Instance& getHandle();

private:
    vk::raii::Instance handle;
};
