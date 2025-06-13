#pragma once

#include <vulkan/vulkan_raii.hpp>

#include "GLFWManager.h"
#include "ImageView.h"

class Framebuffer
{
public:
    Framebuffer(
	vk::raii::Device& device,
	vk::raii::RenderPass& renderPass,
	vk::Image image,
	GLFWwindow* window);

    vk::raii::Framebuffer& getHandle();

private:
    ImageView imageView;
    vk::raii::Framebuffer handle;

};
