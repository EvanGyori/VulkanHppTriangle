#include "Device.h"

#include <vector>
#include <stdexcept>

#include "Utility.h"

namespace
{
const float queuePriority = 1.0f;

int32_t getGraphicsFamilyIndex(
    vk::raii::PhysicalDevice& physicalDevice);

int32_t getPresentFamilyIndex(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::SurfaceKHR& surface);

bool doesPhysicalDeviceSupportRequiredExtensions(
    vk::raii::PhysicalDevice& physicalDevice);

bool doesPhysicalDeviceSupportRequiredSurfaceFormats(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::SurfaceKHR& surface);

bool doesPhysicalDeviceSupportRequiredQueueFamilies(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::SurfaceKHR& surface);

bool isPhysicalDeviceSuitable(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::SurfaceKHR& surface);

vk::raii::PhysicalDevice findSuitablePhysicalDevice(
    vk::raii::Instance& instance,
    vk::raii::SurfaceKHR& surface);

std::vector<vk::DeviceQueueCreateInfo> getQueueCreateInfos(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::SurfaceKHR& surface);

std::vector<const char*> getRequiredExtensions();

std::vector<QueueFamily> getCreatedQueues(
    vk::raii::Device& device,
    const std::vector<vk::DeviceQueueCreateInfo>& queueInfos);
}

Device::Device(vk::raii::Instance& instance, vk::raii::SurfaceKHR& surface) :
    physicalDevice(findSuitablePhysicalDevice(instance, surface)),
    handle(nullptr)
{
    auto queueInfos = getQueueCreateInfos(physicalDevice, surface);
    auto extensions = getRequiredExtensions();
    vk::PhysicalDeviceFeatures features();
    vk::DeviceCreateInfo createInfo({}, queueInfos, {}, extensions, {});
    handle = physicalDevice.createDevice(createInfo);
    //handle = physicalDevice.createDevice({{}, queueInfos, {}, extensions, &features});
    queueFamilies = getCreatedQueues(handle, queueInfos);
}

vk::raii::Queue& Device::getGraphicsQueue()
{
    return queueFamilies[0].queues[0];
}

vk::raii::Queue& Device::getPresentQueue()
{
    // If the graphics and present capabilities weren't in the same family,
    // the present family is stored in queueFamilies[1]
    if (queueFamilies.size() == 2) {
	return queueFamilies[1].queues[0];
    }

    return queueFamilies[0].queues[0];
}

uint32_t Device::getGraphicsQueueFamilyIndex()
{
    return queueFamilies[0].index;
}

uint32_t Device::getPresentQueueFamilyIndex()
{
    // If the graphics and present capabilities weren't in the same family,
    // the present family is stored in queueFamilies[1]
    if (queueFamilies.size() == 2) {
	return queueFamilies[1].index;
    }

    return queueFamilies[0].index;
}

vk::raii::PhysicalDevice& Device::getPhysicalDevice()
{
    return physicalDevice;
}

vk::raii::Device& Device::getHandle()
{
    return handle;
}

namespace
{
int32_t getGraphicsFamilyIndex(
    vk::raii::PhysicalDevice& physicalDevice)
{
    std::vector<vk::QueueFamilyProperties> families = physicalDevice.getQueueFamilyProperties();
    for (int32_t i = 0; i < families.size(); ++i) {
	if (families[i].queueFlags & vk::QueueFlagBits::eGraphics) {
	    return i;
	}
    }

    return -1;
}

int32_t getPresentFamilyIndex(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::SurfaceKHR& surface)
{
    std::vector<vk::QueueFamilyProperties> families = physicalDevice.getQueueFamilyProperties();
    for (int32_t i = 0; i < families.size(); ++i) {
	if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface)) {
	    return i;
	}
    }

    return -1;
}

bool doesPhysicalDeviceSupportRequiredExtensions(
    vk::raii::PhysicalDevice& physicalDevice)
{
    std::vector<vk::ExtensionProperties> supportedExtensions =
	physicalDevice.enumerateDeviceExtensionProperties(nullptr);

    std::vector<const char*> supportedExtensionNames(supportedExtensions.size());
    for (int i = 0; i < supportedExtensions.size(); i++) {
	supportedExtensionNames[i] = supportedExtensions[i].extensionName;
    }

    return isSubset(getRequiredExtensions(), supportedExtensionNames);
}

bool doesPhysicalDeviceSupportRequiredSurfaceFormats(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::SurfaceKHR& surface)
{
    std::vector<vk::SurfaceFormatKHR> formats = physicalDevice.getSurfaceFormatsKHR(surface);
    for (auto format : formats) {
	if (format.format == vk::Format::eR8G8B8A8Srgb &&
	    format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
	{
	    return true;
	}
    }

    return false;
}

bool doesPhysicalDeviceSupportRequiredQueueFamilies(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::SurfaceKHR& surface)
{
    return getGraphicsFamilyIndex(physicalDevice) != -1 &&
	getPresentFamilyIndex(physicalDevice, surface) != -1;
}

bool isPhysicalDeviceSuitable(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::SurfaceKHR& surface)
{
    return doesPhysicalDeviceSupportRequiredExtensions(physicalDevice) &&
	doesPhysicalDeviceSupportRequiredSurfaceFormats(physicalDevice, surface) &&
	doesPhysicalDeviceSupportRequiredQueueFamilies(physicalDevice, surface);
}

vk::raii::PhysicalDevice findSuitablePhysicalDevice(
    vk::raii::Instance& instance,
    vk::raii::SurfaceKHR& surface)
{
    std::vector<vk::raii::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
    for (auto& device : physicalDevices) {
	if (isPhysicalDeviceSuitable(device, surface)) {
	    return device;
	}
    }

    throw std::runtime_error("VULKAN no suitable physical devices found");
    return nullptr;
}

std::vector<vk::DeviceQueueCreateInfo> getQueueCreateInfos(
    vk::raii::PhysicalDevice& physicalDevice,
    vk::raii::SurfaceKHR& surface)
{
    uint32_t graphicsFamilyIndex =
	static_cast<uint32_t>(getGraphicsFamilyIndex(physicalDevice));
    uint32_t presentFamilyIndex =
	static_cast<uint32_t>(getPresentFamilyIndex(physicalDevice, surface));

    int size = (graphicsFamilyIndex == presentFamilyIndex) ? 1 : 2;
    std::vector<vk::DeviceQueueCreateInfo> queueInfos(size);

    queueInfos[0].queueFamilyIndex = graphicsFamilyIndex;
    queueInfos[0].queueCount = 1;
    queueInfos[0].pQueuePriorities = &queuePriority;

    if (graphicsFamilyIndex != presentFamilyIndex) {
	queueInfos[1] = queueInfos[0];
	queueInfos[1].queueFamilyIndex = presentFamilyIndex;
    }

    return queueInfos;
}

std::vector<const char*> getRequiredExtensions()
{
    return { "VK_KHR_swapchain" };
}

std::vector<QueueFamily> getCreatedQueues(
    vk::raii::Device& device,
    const std::vector<vk::DeviceQueueCreateInfo>& queueInfos)
{
    std::vector<QueueFamily> families(queueInfos.size());
    for (int i = 0; i < queueInfos.size(); ++i) {
	families[i].index = queueInfos[i].queueFamilyIndex;
	families[i].queues = {};
	for (uint32_t j = 0; j < queueInfos[i].queueCount; ++j) {
	    families[i].queues.emplace_back(device.getQueue(families[i].index, j));
	}
    }

    return families;
}

}
