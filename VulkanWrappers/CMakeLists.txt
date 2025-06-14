cmake_minimum_required(VERSION 3.20)

include(FindVulkan)

add_library(VulkanWrappers VulkanRaiiContext.cpp VulkanUtility.cpp Instance.cpp DebugMessenger.cpp Surface.cpp Device.cpp BinarySemaphore.cpp Fence.cpp RenderPass.cpp DeviceMemory.cpp Buffer.cpp HostMappedBuffer.cpp VertexBuffer.cpp EmptyPipelineLayout.cpp GraphicsPipeline.cpp ImageView.cpp Framebuffer.cpp Swapchain.cpp CommandPool.cpp GraphicsCommandPool.cpp PresentCommandPool.cpp Renderer.cpp MultisampleImage.cpp)
target_include_directories(VulkanWrappers PUBLIC .)
target_link_libraries(VulkanWrappers PUBLIC GLFWWrappers Vulkan::Vulkan PRIVATE base)
