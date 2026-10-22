cmake_minimum_required(VERSION 3.16)

project(opengl-blog CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SAMPLES "Build sample executables" ON)
option(OGL_COPY_ASSETS "Copy assets/ next to sample binaries after build" ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(framework)

if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
