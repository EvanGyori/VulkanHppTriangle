#include "Config.h"
#include "VertexBuffer.h"

#include <cassert>
#include <cstring>

VertexBuffer::VertexBuffer(
    Device& device,
    vk::DeviceSize size)
    : HostMappedBuffer(device, size, vk::BufferUsageFlagBits::eVertexBuffer),
    device(&device),
    size(size)
{
}

void VertexBuffer::write(const std::vector<Vertex>& vertices)
{
    size_t requiredSize = vertices.size() * sizeof(Vertex);

    if (requiredSize > size) {
	operator=(VertexBuffer(*device, requiredSize));
	//*this = std::move(VertexBuffer(*device, requiredSize));
    }

    assert(size >= requiredSize);

    memcpy(getData(), &vertices[0], requiredSize);
    vk::MappedMemoryRange memoryRange(getMemory(), 0, VK_WHOLE_SIZE);
    device->getHandle().flushMappedMemoryRanges(memoryRange);
}
