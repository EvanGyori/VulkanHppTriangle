#include "Config.h"
#include "GraphicsPipeline.h"

#include <vector>
#include <cstddef>

#include "Utility.h"
#include "Vertex.h"

namespace
{
vk::raii::ShaderModule getShaderModule(
    vk::raii::Device& device,
    const char* filename);

vk::PipelineShaderStageCreateInfo getShaderInfo(
    vk::raii::ShaderModule& shader,
    vk::ShaderStageFlagBits stage);

std::vector<vk::VertexInputBindingDescription> getBindingInfos();

std::vector<vk::VertexInputAttributeDescription> getAttributeInfos();

vk::PipelineInputAssemblyStateCreateInfo getAssemblyInfo();

vk::Viewport getViewport(
    GLFWwindow* window);

vk::Rect2D getScissor(
    GLFWwindow* window);

vk::PipelineRasterizationStateCreateInfo getRasterizationInfo();

vk::PipelineMultisampleStateCreateInfo getMultisampleInfo();

vk::PipelineColorBlendAttachmentState getAttachmentColorBlendInfo();

vk::PipelineColorBlendStateCreateInfo getColorBlendInfo(
    const vk::PipelineColorBlendAttachmentState* attachmentColorBlendInfo);
}

GraphicsPipeline::GraphicsPipeline(
    vk::raii::Device& device,
    vk::raii::RenderPass& renderPass,
    vk::raii::PipelineLayout& layout,
    GLFWwindow* window)
    : handle(nullptr),
    window(window)
{
    auto vertexShader = getShaderModule(device, "vert.spv");
    auto fragmentShader = getShaderModule(device, "frag.spv");
    vk::PipelineShaderStageCreateInfo shaderInfos[2] = {
	getShaderInfo(vertexShader, vk::ShaderStageFlagBits::eVertex),
	getShaderInfo(fragmentShader, vk::ShaderStageFlagBits::eFragment)
    };

    auto bindingInfos = getBindingInfos();
    auto attributeInfos = getAttributeInfos();
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo({}, bindingInfos, attributeInfos);

    auto assemblyInfo = getAssemblyInfo();

    auto viewport = getViewport(window);
    auto scissor = getScissor(window);
    vk::PipelineViewportStateCreateInfo viewportInfo({}, viewport, scissor);

    auto rasterizationInfo = getRasterizationInfo();

    auto multisampleInfo = getMultisampleInfo();

    auto attachmentColorBlendInfo = getAttachmentColorBlendInfo();
    auto colorBlendInfo = getColorBlendInfo(&attachmentColorBlendInfo);

    vk::DynamicState dynamicStates[2] = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    vk::PipelineDynamicStateCreateInfo dynamicInfo({}, dynamicStates);

    vk::GraphicsPipelineCreateInfo createInfo;
    createInfo.stageCount = 2;
    createInfo.pStages = shaderInfos;
    createInfo.pVertexInputState = &vertexInputInfo;
    createInfo.pInputAssemblyState = &assemblyInfo;
    createInfo.pViewportState = &viewportInfo;
    createInfo.pRasterizationState = &rasterizationInfo;
    createInfo.pMultisampleState = &multisampleInfo;
    createInfo.pColorBlendState = &colorBlendInfo;
    createInfo.pDynamicState = &dynamicInfo;
    createInfo.layout = layout;
    createInfo.renderPass = renderPass;
    createInfo.subpass = 0;

    handle = device.createGraphicsPipeline(nullptr, createInfo);
}

vk::raii::Pipeline& GraphicsPipeline::getHandle()
{
    return handle;
}

void GraphicsPipeline::setDynamicStateAndBind(vk::raii::CommandBuffer& commandBuffer)
{
    auto viewport = getViewport(window);
    auto scissor = getScissor(window);

    commandBuffer.setViewport(0, viewport);
    commandBuffer.setScissor(0, scissor);
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, handle);
}

namespace
{
vk::raii::ShaderModule getShaderModule(
    vk::raii::Device& device,
    const char* filename)
{
    std::vector<char> data = loadFile(filename);
    vk::ShaderModuleCreateInfo createInfo(
	{}, data.size(), reinterpret_cast<const uint32_t*>(&data[0]));
    return device.createShaderModule(createInfo);
}

vk::PipelineShaderStageCreateInfo getShaderInfo(
    vk::raii::ShaderModule& shader,
    vk::ShaderStageFlagBits stage)
{
    return {{}, stage, shader, "main"};
}

std::vector<vk::VertexInputBindingDescription> getBindingInfos()
{
    return { { 0, sizeof(Vertex), vk::VertexInputRate::eVertex } };
}

std::vector<vk::VertexInputAttributeDescription> getAttributeInfos()
{
    return {
	{ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) }, // Position attribute
	//{ 1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color) },    // Color attribute
	{ 1, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, textureCoords) } // (u, v) attribute
    };
}

vk::PipelineInputAssemblyStateCreateInfo getAssemblyInfo()
{
    return { {}, vk::PrimitiveTopology::eTriangleList };
}

vk::Viewport getViewport(
    GLFWwindow* window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    return {
	0.0f,
	0.0f,
	static_cast<float>(width),
	static_cast<float>(height),
	0.0f,
	1.0f
    };
}

vk::Rect2D getScissor(
    GLFWwindow* window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    return {
	{ 0, 0 },
	{
	    static_cast<uint32_t>(width),
	    static_cast<uint32_t>(height)
	}
    };
}

vk::PipelineRasterizationStateCreateInfo getRasterizationInfo()
{
    vk::PipelineRasterizationStateCreateInfo info;
    info.polygonMode = vk::PolygonMode::eFill;
    info.cullMode = vk::CullModeFlagBits::eBack;
    info.frontFace = vk::FrontFace::eCounterClockwise;
    info.lineWidth = 1.0f;

    return info;
}

vk::PipelineMultisampleStateCreateInfo getMultisampleInfo()
{
    vk::PipelineMultisampleStateCreateInfo info;
    info.rasterizationSamples = SAMPLE_COUNT_ENUM;
    return info;
}

vk::PipelineColorBlendAttachmentState getAttachmentColorBlendInfo()
{
    vk::PipelineColorBlendAttachmentState info;
    info.colorWriteMask =
	vk::ColorComponentFlagBits::eR |
	vk::ColorComponentFlagBits::eG |
	vk::ColorComponentFlagBits::eB |
	vk::ColorComponentFlagBits::eA;

    return info;
}

vk::PipelineColorBlendStateCreateInfo getColorBlendInfo(
    const vk::PipelineColorBlendAttachmentState* attachmentColorBlendInfo)
{
    vk::PipelineColorBlendStateCreateInfo info;
    info.attachmentCount = 1;
    info.pAttachments = attachmentColorBlendInfo;

    return info;
}

}
