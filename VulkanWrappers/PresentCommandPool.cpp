#include "Config.h"
#include "PresentCommandPool.h"

PresentCommandPool::PresentCommandPool(Device& device) :
    CommandPool(device.getHandle(), device.getPresentQueueFamilyIndex()),
    device(device),
    acquireCommands(nullptr)
{
    vk::CommandBufferAllocateInfo info(getHandle(), vk::CommandBufferLevel::ePrimary, 1);
    acquireCommands = std::move(device.getHandle().allocateCommandBuffers(info)[0]);
}

vk::raii::CommandBuffer& PresentCommandPool::recordAcquireCommands(vk::Image image)
{
    getHandle().reset({});

    vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    acquireCommands.begin(beginInfo);

    vk::ImageSubresourceRange subresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

    vk::ImageMemoryBarrier memoryBarrier(
	{},
	{},
	vk::ImageLayout::ePresentSrcKHR,
	vk::ImageLayout::ePresentSrcKHR,
	device.getGraphicsQueueFamilyIndex(),
	device.getPresentQueueFamilyIndex(),
	image,
	subresourceRange);

    acquireCommands.pipelineBarrier(
	vk::PipelineStageFlagBits::eTopOfPipe,
	vk::PipelineStageFlagBits::eBottomOfPipe,
	{},
	{},
	{},
	memoryBarrier);

    acquireCommands.end();

    return acquireCommands;
}
