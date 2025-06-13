#pragma once

#include "CommandPool.h"
#include "Device.h"

class PresentCommandPool : public CommandPool
{
public:
    PresentCommandPool(Device& device);

    vk::raii::CommandBuffer& recordAcquireCommands(vk::Image image);

private:
    Device& device;
    vk::raii::CommandBuffer acquireCommands;

};
