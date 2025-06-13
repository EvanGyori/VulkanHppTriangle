#include "Config.h"

#include "Instance.h"

#include <vector>
#include <stdexcept>

#include "Utility.h"
#include "GLFWManager.h"
#include "VulkanRaiiContext.h"
#include "DebugMessenger.h"

static std::vector<const char*> getRequiredExtensions();

static std::vector<const char*> getRequiredLayers();

static bool areExtensionsSupported(const std::vector<const char*>& extensions);

static bool areLayersSupported(const std::vector<const char*>& layers);

Instance::Instance() :
    handle(nullptr)
{
    GLFWManager::init();

    vk::ApplicationInfo appInfo({"triangle3", 1, nullptr, 0, VK_API_VERSION_1_0});

    auto layers = getRequiredLayers();
    if (!areLayersSupported(layers)) {
	throw std::runtime_error("VULKAN required layers not supported");
    }

    auto extensions = getRequiredExtensions();
    if (!areExtensionsSupported(extensions)) {
	throw std::runtime_error("VULKAN required extensions not supported");
    }

    vk::InstanceCreateInfo createInfo(
	{{}, &appInfo, layers, extensions});
    
#ifndef NDEBUG
    auto debugMessengerCreateInfo = DebugMessenger::getCreateInfo();
    createInfo.pNext = &debugMessengerCreateInfo;
#endif

    handle = vk::raii::Instance(VulkanRaiiContext::get(), createInfo);
}

vk::raii::Instance& Instance::getHandle()
{
    return handle;
}

static std::vector<const char*> getRequiredExtensions()
{
    uint32_t glfwExtensionCount;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

#ifndef NDEBUG
    extensions.push_back("VK_EXT_debug_utils");
#endif

    return extensions;
}

static std::vector<const char*> getRequiredLayers()
{
#ifdef NDEBUG
    return {};
#else
    return { "VK_LAYER_KHRONOS_validation" };
#endif
}

static bool areExtensionsSupported(const std::vector<const char*>& extensions)
{
    std::vector<vk::ExtensionProperties> supportedExtensions =
	vk::enumerateInstanceExtensionProperties(nullptr);

    std::vector<const char*> supportedExtensionNames(supportedExtensions.size());
    for (int i = 0; i < supportedExtensions.size(); i++) {
	supportedExtensionNames[i] = supportedExtensions[i].extensionName;
    }

    return isSubset(extensions, supportedExtensionNames);
}

static bool areLayersSupported(const std::vector<const char*>& layers)
{
    std::vector<vk::LayerProperties> supportedLayers =
	vk::enumerateInstanceLayerProperties();

    std::vector<const char*> supportedLayerNames(supportedLayers.size());
    for (int i = 0; i < supportedLayers.size(); i++) {
	supportedLayerNames[i] = supportedLayers[i].layerName;
    }

    return isSubset(layers, supportedLayerNames);
}
