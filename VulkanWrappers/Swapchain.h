#pragma once

#include <vector>

#include "Device.h"
#include "Framebuffer.h"
#include "Window.h"
#include "MultisampleImage.h"

class Swapchain
{
public:
    Swapchain(
	Device& device,
	vk::raii::RenderPass& renderPass,
	vk::raii::SurfaceKHR& surface,
	Window& window);

    vk::raii::SwapchainKHR& getHandle();

    vk::Image getImage(uint32_t imageIndex);

    vk::raii::Framebuffer& getFramebuffer(uint32_t imageIndex);

    uint32_t acquireNextImage(vk::raii::Semaphore& signalSemaphore);

    void presentImage(
	uint32_t imageIndex,
	vk::raii::Semaphore& waitSemaphore);

private:
    Device& device;
    vk::raii::RenderPass& renderPass;
    vk::raii::SurfaceKHR& surface;
    GLFWwindow* window;

    vk::raii::SwapchainKHR handle;
    std::vector<vk::Image> images;
    MultisampleImage multisampleImage;
    ImageView multisampleImageView;
    std::vector<Framebuffer> framebuffers;

    void recreate(vk::SwapchainKHR oldSwapchain = nullptr);

    void onWindowSizeChange(int width, int height);

};
