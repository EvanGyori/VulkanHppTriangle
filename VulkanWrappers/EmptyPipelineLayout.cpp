#include "EmptyPipelineLayout.h"

EmptyPipelineLayout::EmptyPipelineLayout(vk::raii::Device& device) :
    handle(device.createPipelineLayout({}))
{
}

vk::raii::PipelineLayout& EmptyPipelineLayout::getHandle()
{
    return handle;
}
