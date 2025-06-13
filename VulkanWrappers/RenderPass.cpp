#include "Config.h"
#include "RenderPass.h"

RenderPass::RenderPass(vk::raii::Device& device) :
    handle(nullptr)
{
    vk::AttachmentDescription attachment(
	{},
	vk::Format::eR8G8B8A8Srgb,
	vk::SampleCountFlagBits::e1,
	vk::AttachmentLoadOp::eClear,
	vk::AttachmentStoreOp::eStore,
	{},
	{},
	vk::ImageLayout::eUndefined,
	vk::ImageLayout::ePresentSrcKHR
    );

    vk::AttachmentReference attachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal);

    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, {}, attachmentReference);

    vk::RenderPassCreateInfo createInfo({}, attachment, subpass);

    handle = device.createRenderPass(createInfo);
}

vk::raii::RenderPass& RenderPass::getHandle()
{
    return handle;
}
