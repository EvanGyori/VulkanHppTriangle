#include "Config.h"
#include "Texture.h"

#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "HostMappedBuffer.h"
#include "Fence.h"

Texture::Texture(
    const char* filename,
    Device& device,
    GraphicsCommandPool& commandPool)
    : image(nullptr)
{
    // Load image from file
    int width, height, numComponentsPerPixel;
    unsigned char* data = stbi_load(filename, &width, &height, &numComponentsPerPixel, 4);
    assert(data != nullptr);
    int size = width * height * 4;

    // Put image data into a buffer in device memory
    HostMappedBuffer buffer(device, size, vk::BufferUsageFlagBits::eTransferSrc);
    memcpy(buffer.getData(), data, size);
    vk::MappedMemoryRange memoryRange(buffer.getMemory(), 0, VK_WHOLE_SIZE);
    device.getHandle().flushMappedMemoryRanges(memoryRange);
    stbi_image_free(data);

    // Create image object, memory for it, and image view
    image = Image(device, static_cast<uint32_t>(width), static_cast<uint32_t>(height),
	vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst);

    // transition image to proper layout and copy data from buffer to actual
    // image object using a graphics queue
    vk::CommandBuffer commandBuffer = 
	commandPool.recordTransferCommands(buffer.getHandle(), image.getHandle(),
	    static_cast<uint32_t>(width), static_cast<uint32_t>(height));

    Fence fence(device.getHandle(), false);
    
    vk::SubmitInfo submitInfo({}, {}, commandBuffer, {});
    device.getGraphicsQueue().submit(submitInfo, fence.getHandle());

    fence.wait();
}

vk::raii::ImageView& Texture::getImageView()
{
    return image.getImageView();
}
