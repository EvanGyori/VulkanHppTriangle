#pragma once

#include "GLFWManager.h"

#include "CommandPool.h"
#include "GraphicsPipeline.h"
#include "Device.h"

class GraphicsCommandPool : public CommandPool
{
public:
    GraphicsCommandPool(Device& device);

    vk::raii::CommandBuffer& recordDrawCommands(
	vk::raii::RenderPass& renderPass,
	vk::raii::Framebuffer& framebuffer,
	vk::Image image,
	GLFWwindow* window,
	GraphicsPipeline& pipeline,
	vk::PipelineLayout pipelineLayout,
	vk::DescriptorSet descriptorSet,
	vk::Buffer vertexBuffer,
	uint32_t vertexCount);

    // Copies data from the source buffer to the destination image
    vk::raii::CommandBuffer& recordTransferCommands(
	vk::raii::Buffer& srcBuffer,
	vk::raii::Image& dstImage,
	uint32_t imageWidth,
	uint32_t imageHeight);

private:
    Device& device;
    // Multi-use, reset and recorded into each time a record* method is called
    vk::raii::CommandBuffer commandBuffer;
};
