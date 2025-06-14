#include "Config.h"
#include "Renderer.h"

#include <iostream>

Renderer::Renderer() :
    window("VulkanTriangle3", 500, 500),
    surface(instance.getHandle(), window.getHandle()),
    device(instance.getHandle(), surface.getHandle()),
    acquiredImageSemaphore(device.getHandle()),
    drawedSemaphore(device.getHandle()),
    transferredSemaphore(device.getHandle()),
    frameFence(device.getHandle(), true),
    renderPass(device.getHandle()),
    vertexBuffer(device, 100),
    pipelineLayout(device.getHandle()),
    pipeline(device.getHandle(), renderPass.getHandle(),
	pipelineLayout.getHandle(), window.getHandle()),
    swapchain(device, renderPass.getHandle(),
	surface.getHandle(), window),
    graphicsCommandPool(device),
    presentCommandPool(device),
#ifndef NDEBUG
    debugger(instance.getHandle())
#endif
{
}

Renderer::~Renderer()
{
    device.getHandle().waitIdle();
}

void Renderer::draw(const std::vector<Vertex>& vertices)
{
    // wait till previous frame has finished rendering
    frameFence.wait();
    frameFence.reset();

    // Must wait for fence since this may result in a window changed size callback
    // which would recreate the swapchain even though it's images might still be in use
    glfwPollEvents();

    int width, height;
    glfwGetFramebufferSize(window.getHandle(), &width, &height);
    // Don't draw to nothing when window is minimized
    if (width > 0 && height > 0) {
	bool singleQueueFamily =
	    device.getGraphicsQueueFamilyIndex() == device.getPresentQueueFamilyIndex();

	vk::Semaphore acquiredImageSemaphoreHandle = acquiredImageSemaphore.getHandle();
	vk::Semaphore drawedSemaphoreHandle = drawedSemaphore.getHandle();
	vk::Semaphore transferredSemaphoreHandle = transferredSemaphore.getHandle();

	vk::Fence fenceHandle = frameFence.getHandle();

	// acquire next image
	uint32_t imageIndex = swapchain.acquireNextImage(acquiredImageSemaphore.getHandle());

	// write vertices to buffer
	vertexBuffer.write(vertices);

	// draw
	vk::CommandBuffer drawCommands = graphicsCommandPool.recordDrawCommands(
	    renderPass.getHandle(), swapchain.getFramebuffer(imageIndex), swapchain.getImage(imageIndex),
	    window.getHandle(), pipeline, vertexBuffer.getHandle(), vertices.size());
       
	vk::PipelineStageFlags waitStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	vk::SubmitInfo drawSubmitInfo(acquiredImageSemaphoreHandle, waitStage,
	    drawCommands, drawedSemaphoreHandle);
	device.getGraphicsQueue().submit(drawSubmitInfo,
	    singleQueueFamily ? fenceHandle : nullptr);

	// acquire exlusive ownership of image on present queue
	if (!singleQueueFamily) {
	    vk::CommandBuffer acquireCommands =
		presentCommandPool.recordAcquireCommands(swapchain.getImage(imageIndex));
	    waitStage = vk::PipelineStageFlagBits::eAllCommands;
	    vk::SubmitInfo acquireSubmitInfo(drawedSemaphoreHandle, waitStage,
		acquireCommands, transferredSemaphoreHandle);
	    device.getPresentQueue().submit(acquireSubmitInfo, frameFence.getHandle());
	}

	swapchain.presentImage(imageIndex, 
	    singleQueueFamily ? drawedSemaphore.getHandle() : transferredSemaphore.getHandle());
    }
}

bool Renderer::isRunning()
{
    return glfwWindowShouldClose(window.getHandle()) == 0;
}
