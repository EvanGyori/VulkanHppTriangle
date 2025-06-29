cmake_minimum_required(VERSION 3.20)
project(VulkanTriangle3)

function(add_shader target shaderName shaderOutputName)
    add_custom_command(TARGET ${target} POST_BUILD
	COMMAND glslc "${CMAKE_SOURCE_DIR}/Shaders/${shaderName}"
	-o "$<TARGET_FILE_DIR:${target}>/${shaderOutputName}")
endfunction()

add_subdirectory(stb)
add_subdirectory(base)
add_subdirectory(GLFWWrappers)
add_subdirectory(VulkanWrappers)

#add_executable(triangle main.cpp vert.spv frag.spv)
add_executable(triangle main.cpp)
target_link_libraries(triangle PRIVATE GLFWWrappers VulkanWrappers)

#add_custom_command(TARGET triangle POST_BUILD
#COMMAND glslc "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/shader.vert" -o "$<TARGET_FILE_DIR:triangle>/vert.spv")
add_custom_command(TARGET triangle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/gunEmoji.jpg"
    "$<TARGET_FILE_DIR:triangle>/gunEmoji.jpg"
)

add_shader(triangle shader.vert vert.spv)
add_shader(triangle shader.frag frag.spv)
