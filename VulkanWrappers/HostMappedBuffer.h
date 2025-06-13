#pragma once

#include "Buffer.h"

class HostMappedBuffer : public Buffer
{
public:
    HostMappedBuffer(
	Device& device,
	vk::DeviceSize size,
	vk::BufferUsageFlags usage);

    void* getData();

private:
    void* data;
};
