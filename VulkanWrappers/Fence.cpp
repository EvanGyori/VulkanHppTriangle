#include "Fence.h"

#include <limits>

Fence::Fence(vk::raii::Device& device, bool startSignaled) :
    device(device),
    handle(nullptr)
{
    vk::FenceCreateInfo createInfo;
    if (startSignaled) {
	createInfo.flags = vk::FenceCreateFlagBits::eSignaled;
    }

    handle = device.createFence(createInfo);
}

void Fence::wait()
{
    vk::Fence fenceHandle = handle;
    device.waitForFences(fenceHandle, true, std::numeric_limits<uint64_t>::max());
}

void Fence::reset()
{
    vk::Fence fenceHandle = handle;
    device.resetFences(fenceHandle);
}

vk::raii::Fence& Fence::getHandle()
{
    return handle;
}
