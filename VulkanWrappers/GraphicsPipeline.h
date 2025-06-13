#pragma once

#include <vulkan/vulkan_raii.hpp>
#include "GLFWManager.h"

class GraphicsPipeline
{
public:
    GraphicsPipeline(
	vk::raii::Device& device,
	vk::raii::RenderPass& renderPass,
	vk::raii::PipelineLayout& layout,
	GLFWwindow* window);

    vk::raii::Pipeline& getHandle();

    void setDynamicStateAndBind(vk::raii::CommandBuffer& commandBuffer);

private:
    vk::raii::Pipeline handle;
    GLFWwindow* window;

};
