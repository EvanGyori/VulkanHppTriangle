#include "Config.h"
#include "Framebuffer.h"

Framebuffer::Framebuffer(
    vk::raii::Device& device,
    vk::raii::RenderPass& renderPass,
    vk::ImageView multisampleImageView,
    vk::Image image,
    GLFWwindow* window)
    : imageView(device, image),
    handle(nullptr)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    vk::ImageView viewHandles[2] = { multisampleImageView, imageView.getHandle() };

    vk::FramebufferCreateInfo createInfo(
	{},
	renderPass,
	viewHandles,
	static_cast<uint32_t>(width),
	static_cast<uint32_t>(height),
	1
    );

    handle = device.createFramebuffer(createInfo);
}

vk::raii::Framebuffer& Framebuffer::getHandle()
{
    return handle;
}
