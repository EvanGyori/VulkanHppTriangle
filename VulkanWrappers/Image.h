#pragma once

#include "DeviceMemory.h"
#include "ImageView.h"

class Image
{
public:
    Image(nullptr_t);

    Image(
	Device& device,
	uint32_t width,
	uint32_t height,
	vk::ImageUsageFlags usage);

    vk::raii::Image& getHandle();

    vk::raii::ImageView& getImageView();

private:
    vk::raii::Image handle;
    DeviceMemory memory;
    ImageView view;

};
