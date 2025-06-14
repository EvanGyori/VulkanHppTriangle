#include "Config.h"
#include "MultisampleImage.h"

MultisampleImage::MultisampleImage(nullptr_t) :
    handle(nullptr),
    memory(nullptr)
{
}

MultisampleImage::MultisampleImage(
    Device& device,
    vk::Extent2D extent)
    : handle(nullptr),
    memory(nullptr)
{
    vk::Extent3D extent3D = {
	extent.width,
	extent.height,
	1
    };

    vk::ImageCreateInfo createInfo(
	{},
	vk::ImageType::e2D,
	vk::Format::eR8G8B8A8Srgb,
	extent3D,
	1,
	1,
	SAMPLE_COUNT_ENUM,
	vk::ImageTiling::eOptimal,
	vk::ImageUsageFlagBits::eColorAttachment,
	vk::SharingMode::eExclusive,
	{},
	vk::ImageLayout::eUndefined
    );

    handle = device.getHandle().createImage(createInfo);

    vk::MemoryRequirements requirements = handle.getMemoryRequirements();

    memory = DeviceMemory(device, requirements.size, requirements.memoryTypeBits, {},
	vk::MemoryPropertyFlagBits::eDeviceLocal);

    handle.bindMemory(memory.getHandle(), 0);
}

vk::raii::Image& MultisampleImage::getHandle()
{
    return handle;
}
