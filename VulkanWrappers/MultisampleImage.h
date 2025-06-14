#pragma once

#include "DeviceMemory.h"
#include "Device.h"

class MultisampleImage
{
public:
    MultisampleImage(nullptr_t);

    MultisampleImage(
	Device& device,
	vk::Extent2D extent);

    vk::raii::Image& getHandle();

private:
    vk::raii::Image handle;
    DeviceMemory memory;

};
