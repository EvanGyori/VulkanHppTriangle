#pragma once

#include <vector>

#include "Window.h"
#include "Instance.h"
#include "Surface.h"
#include "Device.h"
#include "BinarySemaphore.h"
#include "Fence.h"
#include "RenderPass.h"
#include "VertexBuffer.h"
#include "EmptyPipelineLayout.h"
#include "GraphicsPipeline.h"
#include "Swapchain.h"
#include "GraphicsCommandPool.h"
#include "PresentCommandPool.h"

#ifndef NDEBUG
#include "DebugMessenger.h"
#endif

class Renderer
{
public:
    Renderer();

    ~Renderer();

    void draw(const std::vector<Vertex>& vertices);

    // Returns false if the window has been closed
    bool isRunning();

private:
    Window window;
    Instance instance;
    Surface surface;
    Device device;
    BinarySemaphore acquiredImageSemaphore, drawedSemaphore, transferredSemaphore;
    Fence frameFence;
    RenderPass renderPass;
    VertexBuffer vertexBuffer;
    EmptyPipelineLayout pipelineLayout;
    GraphicsPipeline pipeline;
    Swapchain swapchain;
    GraphicsCommandPool graphicsCommandPool;
    PresentCommandPool presentCommandPool;

#ifndef NDEBUG
    DebugMessenger debugger;
#endif

};
