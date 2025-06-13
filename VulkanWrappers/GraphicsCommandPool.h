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
	vk::Buffer vertexBuffer,
	uint32_t vertexCount);

private:
    Device& device;
    vk::raii::CommandBuffer drawCommands;
};
