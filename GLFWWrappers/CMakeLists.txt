cmake_minimum_required(VERSION 3.20)

find_package(glfw NAMES glfw3 GLFW REQUIRED)

add_library(GLFWWrappers GLFWManager.cpp Window.cpp)
target_include_directories(GLFWWrappers PUBLIC .)
target_link_libraries(GLFWWrappers PUBLIC glfw PRIVATE base)
