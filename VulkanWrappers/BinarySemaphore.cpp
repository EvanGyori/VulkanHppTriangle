#include "BinarySemaphore.h"

BinarySemaphore::BinarySemaphore(vk::raii::Device& device) :
    handle(nullptr)
{
    vk::SemaphoreCreateInfo createInfo;
    handle = device.createSemaphore(createInfo);
}

vk::raii::Semaphore& BinarySemaphore::getHandle()
{
    return handle;
}
