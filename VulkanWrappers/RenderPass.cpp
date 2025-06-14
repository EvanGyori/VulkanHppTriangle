#include "Config.h"
#include "RenderPass.h"

RenderPass::RenderPass(vk::raii::Device& device) :
    handle(nullptr)
{
    vk::AttachmentDescription colorAttachment(
	{},
	vk::Format::eR8G8B8A8Srgb,
	SAMPLE_COUNT_ENUM,
	vk::AttachmentLoadOp::eClear,
	vk::AttachmentStoreOp::eDontCare,
	{},
	{},
	vk::ImageLayout::eUndefined,
	vk::ImageLayout::eColorAttachmentOptimal
    );

    vk::AttachmentDescription resolveAttachment(
	{},
	vk::Format::eR8G8B8A8Srgb,
	vk::SampleCountFlagBits::e1,
	vk::AttachmentLoadOp::eDontCare,
	vk::AttachmentStoreOp::eStore,
	{},
	{},
	vk::ImageLayout::eUndefined,
	vk::ImageLayout::ePresentSrcKHR
    );
    
    vk::AttachmentDescription attachments[2] = { colorAttachment, resolveAttachment };

    vk::AttachmentReference colorAttachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal);
    vk::AttachmentReference resolveAttachmentReference(1, vk::ImageLayout::eColorAttachmentOptimal);

    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, {},
	colorAttachmentReference, resolveAttachmentReference);

    vk::RenderPassCreateInfo createInfo({}, attachments, subpass);

    handle = device.createRenderPass(createInfo);
}

vk::raii::RenderPass& RenderPass::getHandle()
{
    return handle;
}
