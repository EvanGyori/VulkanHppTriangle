#pragma once

#include "Device.h"
#include "GraphicsCommandPool.h"
#include "Image.h"

class Texture
{
public:
    Texture(
	const char* filename,
	Device& device,
	GraphicsCommandPool& commandPool);

    vk::raii::ImageView& getImageView();

private:
    Image image;

};
