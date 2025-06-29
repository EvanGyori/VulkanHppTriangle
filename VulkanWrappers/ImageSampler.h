#pragma once

#include <vulkan/vulkan_raii.hpp>

class ImageSampler
{
public:
    ImageSampler(vk::raii::Device& device);

    vk::raii::Sampler& getHandle();

private:
    vk::raii::Sampler handle;
};
