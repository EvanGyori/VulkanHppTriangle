#pragma once

#include <vulkan/vulkan_raii.hpp>

class ImageView
{
public:
    ImageView(
	vk::raii::Device& device,
	vk::Image image);

    vk::raii::ImageView& getHandle();

private:
    vk::raii::ImageView handle;

};
