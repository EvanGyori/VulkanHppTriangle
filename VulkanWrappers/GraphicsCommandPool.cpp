#include "Config.h"
#include "GraphicsCommandPool.h"

GraphicsCommandPool::GraphicsCommandPool(Device& device) :
    CommandPool(device.getHandle(), device.getGraphicsQueueFamilyIndex()),
    device(device),
    drawCommands(nullptr)
{
    vk::CommandBufferAllocateInfo info(getHandle(), vk::CommandBufferLevel::ePrimary, 1);
    std::vector<vk::raii::CommandBuffer> buffers = device.getHandle().allocateCommandBuffers(info);
    drawCommands = std::move(buffers[0]);
}

vk::raii::CommandBuffer& GraphicsCommandPool::recordDrawCommands(
    vk::raii::RenderPass& renderPass,
    vk::raii::Framebuffer& framebuffer,
    vk::Image image,
    GLFWwindow* window,
    GraphicsPipeline& pipeline,
    vk::Buffer vertexBuffer,
    uint32_t vertexCount)
{
    // Reset command pool
    getHandle().reset({});

    vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    drawCommands.begin(beginInfo);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    vk::Rect2D renderArea(
	{ 0, 0 },
	{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) });

    vk::ClearValue clearValue;

    vk::RenderPassBeginInfo renderPassInstanceInfo(renderPass, framebuffer, renderArea, clearValue);
    drawCommands.beginRenderPass(renderPassInstanceInfo, vk::SubpassContents::eInline);

    pipeline.setDynamicStateAndBind(drawCommands);

    vk::DeviceSize zeroOffset = 0;
    drawCommands.bindVertexBuffers(0, vertexBuffer, zeroOffset);
    drawCommands.draw(vertexCount, 1, 0, 0);

    drawCommands.endRenderPass();

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

	drawCommands.pipelineBarrier(
	    vk::PipelineStageFlagBits::eColorAttachmentOutput,
	    vk::PipelineStageFlagBits::eBottomOfPipe,
	    {},
	    {},
	    {},
	    memoryBarrier);
    }

    drawCommands.end();

    return drawCommands; 
}
