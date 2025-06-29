#include "Config.h"
#include "GraphicsCommandPool.h"

GraphicsCommandPool::GraphicsCommandPool(Device& device) :
    CommandPool(device.getHandle(), device.getGraphicsQueueFamilyIndex()),
    device(device),
    commandBuffer(nullptr)
{
    vk::CommandBufferAllocateInfo info(getHandle(), vk::CommandBufferLevel::ePrimary, 1);
    std::vector<vk::raii::CommandBuffer> buffers = device.getHandle().allocateCommandBuffers(info);
    commandBuffer = std::move(buffers[0]);
}

vk::raii::CommandBuffer& GraphicsCommandPool::recordDrawCommands(
    vk::raii::RenderPass& renderPass,
    vk::raii::Framebuffer& framebuffer,
    vk::Image image,
    GLFWwindow* window,
    GraphicsPipeline& pipeline,
    vk::PipelineLayout pipelineLayout,
    vk::DescriptorSet descriptorSet,
    vk::Buffer vertexBuffer,
    uint32_t vertexCount)
{
    // Reset command pool
    getHandle().reset({});

    vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    commandBuffer.begin(beginInfo);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    vk::Rect2D renderArea(
	{ 0, 0 },
	{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) });

    vk::ClearValue clearValue;

    vk::RenderPassBeginInfo renderPassInstanceInfo(renderPass, framebuffer, renderArea, clearValue);
    commandBuffer.beginRenderPass(renderPassInstanceInfo, vk::SubpassContents::eInline);

    pipeline.setDynamicStateAndBind(commandBuffer);

    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayout, 0,
	descriptorSet, {});

    vk::DeviceSize zeroOffset = 0;
    commandBuffer.bindVertexBuffers(0, vertexBuffer, zeroOffset);
    commandBuffer.draw(vertexCount, 1, 0, 0);

    commandBuffer.endRenderPass();

    // Release exclusive ownership if presenting is performed on separate queue family
    if (device.getGraphicsQueueFamilyIndex() != device.getPresentQueueFamilyIndex()) {
	vk::ImageSubresourceRange subresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

	vk::ImageMemoryBarrier memoryBarrier(
	    vk::AccessFlagBits::eColorAttachmentWrite,
	    {},
	    vk::ImageLayout::ePresentSrcKHR,
	    vk::ImageLayout::ePresentSrcKHR,
	    device.getGraphicsQueueFamilyIndex(),
	    device.getPresentQueueFamilyIndex(),
	    image,
	    subresourceRange);

	commandBuffer.pipelineBarrier(
	    vk::PipelineStageFlagBits::eColorAttachmentOutput,
	    vk::PipelineStageFlagBits::eBottomOfPipe,
	    {},
	    {},
	    {},
	    memoryBarrier);
    }

    commandBuffer.end();
    return commandBuffer; 
}

vk::raii::CommandBuffer& GraphicsCommandPool::recordTransferCommands(
    vk::raii::Buffer& srcBuffer,
    vk::raii::Image& dstImage,
    uint32_t imageWidth,
    uint32_t imageHeight)
{
    getHandle().reset({});

    vk::CommandBufferBeginInfo beginInfo(
	vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    commandBuffer.begin(beginInfo);

    vk::ImageSubresourceRange subresource(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

    // Transition image to optimal destination transfer layout
    vk::ImageMemoryBarrier initialBarrier(
	{},
	vk::AccessFlagBits::eTransferWrite,
	vk::ImageLayout::eUndefined,
	vk::ImageLayout::eTransferDstOptimal,
	device.getGraphicsQueueFamilyIndex(),
	device.getGraphicsQueueFamilyIndex(),
	dstImage,
	subresource);

    commandBuffer.pipelineBarrier(
	vk::PipelineStageFlagBits::eTopOfPipe,
	vk::PipelineStageFlagBits::eTransfer,
	{},
	{},
	{},
	initialBarrier);
    
    // Copy data from buffer to image
    vk::ImageSubresourceLayers subresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);

    vk::Extent3D extent {
	imageWidth,
	imageHeight,
	1
    };

    vk::BufferImageCopy region(
	0, 0, 0,
	subresourceLayers,
	{},
	extent);

    commandBuffer.copyBufferToImage(srcBuffer, dstImage, vk::ImageLayout::eTransferDstOptimal, region);

    // Transition image to optimal shader read only for descriptors layout
    vk::ImageMemoryBarrier finalBarrier(
	vk::AccessFlagBits::eTransferWrite,
	{},
	vk::ImageLayout::eTransferDstOptimal,
	vk::ImageLayout::eShaderReadOnlyOptimal,
	device.getGraphicsQueueFamilyIndex(),
	device.getGraphicsQueueFamilyIndex(),
	dstImage,
	subresource);

    commandBuffer.pipelineBarrier(
	vk::PipelineStageFlagBits::eTransfer,
	vk::PipelineStageFlagBits::eBottomOfPipe,
	{},
	{},
	{},
	finalBarrier);

    commandBuffer.end();
    return commandBuffer;
}
