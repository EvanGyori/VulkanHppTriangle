#include "Config.h"
#include "ImageSampler.h"

ImageSampler::ImageSampler(vk::raii::Device& device) :
    handle(nullptr)
{
    vk::SamplerCreateInfo createInfo(
	{},
	vk::Filter::eNearest,
	vk::Filter::eNearest,
	{}, //vk::SamplerMipmapNode::eNearest,
	vk::SamplerAddressMode::eRepeat,
	vk::SamplerAddressMode::eRepeat,
	vk::SamplerAddressMode::eRepeat,
	0.0f,
	false,
	0.0f,
	false,
	{},
	0.0f,
	0.0f,
	{},
	false
    );

    handle = device.createSampler(createInfo);
}

vk::raii::Sampler& ImageSampler::getHandle()
{
    return handle;
}
