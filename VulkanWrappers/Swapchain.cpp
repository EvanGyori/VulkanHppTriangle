#include "Config.h"
#include "Swapchain.h"

#include <limits>

namespace
{
uint32_t getMinImageCount(
    vk::SurfaceCapabilitiesKHR capabilities);

vk::Extent2D getImageExtent(
    vk::SurfaceCapabilitiesKHR capabilities,
    GLFWwindow* window);

vk::SurfaceTransformFlagBitsKHR getPreTransform(
    vk::SurfaceCapabilitiesKHR capabilities);

vk::CompositeAlphaFlagBitsKHR getCompositeAlpha(
    vk::SurfaceCapabilitiesKHR capabilities);
}

Swapchain::Swapchain(
    Device& device,
    vk::raii::RenderPass& renderPass,
    vk::raii::SurfaceKHR& surface,
    Window& window)
    : device(device),
    renderPass(renderPass),
    surface(surface),
    window(window.getHandle()),
    handle(nullptr),
    multisampleImage(nullptr),
    multisampleImageView(nullptr)
{
    recreate();
    window.subscribeToFramebufferSizeEvent([this](int width, int height) {
	this->onWindowSizeChange(width, height);
    });
}

vk::raii::SwapchainKHR& Swapchain::getHandle()
{
    return handle;
}

vk::Image Swapchain::getImage(uint32_t imageIndex)
{
    return images[imageIndex];
}

vk::raii::Framebuffer& Swapchain::getFramebuffer(uint32_t imageIndex)
{
    return framebuffers[imageIndex].getHandle();
}

uint32_t Swapchain::acquireNextImage(vk::raii::Semaphore& signalSemaphore)
{
    return handle.acquireNextImage(std::numeric_limits<uint64_t>::max(), signalSemaphore).second;
}

void Swapchain::presentImage(
    uint32_t imageIndex,
    vk::raii::Semaphore& waitSemaphore)
{
    auto semaphoreHandle = static_cast<vk::Semaphore>(waitSemaphore);
    auto swapchainHandle = static_cast<vk::SwapchainKHR>(handle);
    vk::PresentInfoKHR info(semaphoreHandle, swapchainHandle, imageIndex);
    device.getPresentQueue().presentKHR(info);
}

void Swapchain::recreate(vk::SwapchainKHR oldSwapchain)
{
    vk::SurfaceCapabilitiesKHR capabilities =
	device.getPhysicalDevice().getSurfaceCapabilitiesKHR(surface);

    auto extent = getImageExtent(capabilities, window);

    // Don't create swapchain if window is minimized
    if (extent.width > 0 && extent.height > 0) {
	vk::SwapchainCreateInfoKHR createInfo(
	    {},
	    surface,
	    getMinImageCount(capabilities),
	    vk::Format::eR8G8B8A8Srgb,
	    vk::ColorSpaceKHR::eSrgbNonlinear,
	    extent,
	    1,
	    vk::ImageUsageFlagBits::eColorAttachment,
	    vk::SharingMode::eExclusive,
	    {},
	    getPreTransform(capabilities),
	    getCompositeAlpha(capabilities),
	    vk::PresentModeKHR::eFifo,
	    true,
	    oldSwapchain);

	handle = device.getHandle().createSwapchainKHR(createInfo);

	multisampleImage = MultisampleImage(device, extent);
	multisampleImageView = ImageView(device.getHandle(), multisampleImage.getHandle());

	images = handle.getImages();
	framebuffers = std::vector<Framebuffer>();

	for (int i = 0; i < images.size(); ++i) {
	    framebuffers.emplace_back(device.getHandle(), renderPass,
		multisampleImageView.getHandle(), images[i], window);
	}
    }
}

void Swapchain::onWindowSizeChange(int width, int height)
{
    recreate(handle);
}

namespace
{
uint32_t getMinImageCount(
    vk::SurfaceCapabilitiesKHR capabilities)
{
    uint32_t count = 2;
    if (count < capabilities.minImageCount) {
	count = capabilities.minImageCount;
    }

    if (capabilities.maxImageCount != 0 && count > capabilities.maxImageCount) {
	count = capabilities.maxImageCount;
    }

    return count;
}

vk::Extent2D getImageExtent(
    vk::SurfaceCapabilitiesKHR capabilities,
    GLFWwindow* window)
{
    vk::Extent2D extent = capabilities.currentExtent;
    if (extent.width == 0xFFFFFFFF &&
	extent.height == 0xFFFFFFFF) {
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	extent = vk::Extent2D {
	    static_cast<uint32_t>(width),
	    static_cast<uint32_t>(height)
	};
    }

    return extent;
}

vk::SurfaceTransformFlagBitsKHR getPreTransform(
    vk::SurfaceCapabilitiesKHR capabilities)
{
    return capabilities.currentTransform;
}

vk::CompositeAlphaFlagBitsKHR getCompositeAlpha(
    vk::SurfaceCapabilitiesKHR capabilities)
{
    auto mask = capabilities.supportedCompositeAlpha;
    if (mask & vk::CompositeAlphaFlagBitsKHR::eOpaque) {
	return vk::CompositeAlphaFlagBitsKHR::eOpaque;
    } else if (mask & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) {
	return vk::CompositeAlphaFlagBitsKHR::ePreMultiplied;
    } else if (mask & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) {
	return vk::CompositeAlphaFlagBitsKHR::ePostMultiplied;
    } else {
	return vk::CompositeAlphaFlagBitsKHR::eInherit;
    }
}

}
