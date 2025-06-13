cmake_minimum_required(VERSION 3.20)

add_library(base Utility.cpp)
target_include_directories(base
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}"
    INTERFACE .)

target_compile_definitions(base PUBLIC $<$<CONFIG:Release>:NDEBUG>)
configure_file(Config.h.in Config.h)
