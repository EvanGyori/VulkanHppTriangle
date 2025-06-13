#include "Config.h"

#include "DebugMessenger.h"

#include <string>
#include <iostream>

#include "VulkanRaiiContext.h"

static const char* getMessageSeverityString(VkDebugUtilsMessageSeverityFlagBitsEXT severityFlag);

static std::string getMessageTypesString(VkDebugUtilsMessageTypeFlagsEXT typeFlags);

static VkBool32 VKAPI_PTR debugMessengerCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT types,
	const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
	void* userData);

DebugMessenger::DebugMessenger(vk::raii::Instance& instance) :
    handle(instance, getCreateInfo())
{
}

vk::DebugUtilsMessengerCreateInfoEXT DebugMessenger::getCreateInfo()
{
    vk::DebugUtilsMessengerCreateInfoEXT createInfo;
    createInfo.messageSeverity =
	//vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
	//vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
	vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
	vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    createInfo.messageType =
	vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
	vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
	vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
    createInfo.pfnUserCallback =
	reinterpret_cast<vk::PFN_DebugUtilsMessengerCallbackEXT>(debugMessengerCallback);

    return createInfo;
}

static const char* getMessageSeverityString(VkDebugUtilsMessageSeverityFlagBitsEXT severityFlag)
{
    switch (severityFlag) {
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
	    return "verbose";
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
	    return "info";
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
	    return "WARNING";
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
	    return "ERROR";
	default:
	    return "unknownSeverity";
    }
}

static std::string getMessageTypesString(VkDebugUtilsMessageTypeFlagsEXT typeFlags)
{
    std::string types = "";

    if (typeFlags & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) {
	types = "general";
    }

    if (typeFlags & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) {
	if (types != "") {
	    types += ", ";
	}
	types += "validation";
    }

    if (typeFlags & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) {
	if (types != "") {
	    types += ", ";
	}
	types += "performance";
    }

    return (types == "") ? "unknownType" : types;
}

static VkBool32 VKAPI_PTR debugMessengerCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT severity,
	VkDebugUtilsMessageTypeFlagsEXT types,
	const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
	void* userData)
{
    std::cout << "VULKAN: " << getMessageSeverityString(severity)
	<< " TYPES: " << getMessageTypesString(types)
	<< " MESSAGE: " << callbackData->pMessage << std::endl;

    return VK_FALSE;
}
