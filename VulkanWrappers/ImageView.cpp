#include "Config.h"
#include "ImageView.h"

ImageView::ImageView(
    vk::raii::Device& device,
    vk::Image image)
    : handle(nullptr)
{
    vk::ComponentMapping swizzle(
	vk::ComponentSwizzle::eIdentity,
	vk::ComponentSwizzle::eIdentity,
	vk::ComponentSwizzle::eIdentity,
	vk::ComponentSwizzle::eIdentity);

    vk::ImageSubresourceRange subresource(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);
    
    vk::ImageViewCreateInfo createInfo(
	{}, image, vk::ImageViewType::e2D, vk::Format::eR8G8B8A8Srgb, swizzle, subresource);

    handle = device.createImageView(createInfo);
}

vk::raii::ImageView& ImageView::getHandle()
{
    return handle;
}
