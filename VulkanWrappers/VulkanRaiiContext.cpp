#include "VulkanRaiiContext.h"

VulkanRaiiContext* VulkanRaiiContext::instance = nullptr;

vk::raii::Context& VulkanRaiiContext::get()
{
    if (instance == nullptr) {
	instance = new VulkanRaiiContext;
    }

    return instance->context;
}
