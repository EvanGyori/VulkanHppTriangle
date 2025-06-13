#pragma once

#include <vector>

#include "HostMappedBuffer.h"
#include "Vertex.h"

class VertexBuffer : public HostMappedBuffer
{
public:
    VertexBuffer(
	Device& device,
	vk::DeviceSize size = 0);

    void write(const std::vector<Vertex>& vertices);

private:
    Device* device;
    vk::DeviceSize size;
};
