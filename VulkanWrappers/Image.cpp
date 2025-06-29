#include "Config.h"
#include "Image.h"

Image::Image(nullptr_t) :
    handle(nullptr),
    memory(nullptr),
    view(nullptr)
{
}

Image::Image(
    Device& device,
    uint32_t width,
    uint32_t height,
    vk::ImageUsageFlags usage)
    : handle(nullptr),
    memory(nullptr),
    view(nullptr)
{
    // Create image object
    vk::Extent3D extent {
	width,
	height,
	1
    };

    vk::ImageCreateInfo createInfo(
	{},
	vk::ImageType::e2D,
	vk::Format::eR8G8B8A8Srgb,
	extent,
	1,
	1,
	vk::SampleCountFlagBits::e1,
	vk::ImageTiling::eOptimal,
	usage,
	vk::SharingMode::eExclusive,
	{},
	vk::ImageLayout::eUndefined);

    handle = device.getHandle().createImage(createInfo);

    // Create memory object
    vk::MemoryRequirements requirements = handle.getMemoryRequirements();
    memory = DeviceMemory(device, requirements.size, requirements.memoryTypeBits,
	{}, vk::MemoryPropertyFlagBits::eDeviceLocal);

    // Bind image to memory
    handle.bindMemory(memory.getHandle(), 0);

    // Create image view
    view = ImageView(device.getHandle(), handle);
}

vk::raii::Image& Image::getHandle()
{
    return handle;
}

vk::raii::ImageView& Image::getImageView()
{
    return view.getHandle();
}
