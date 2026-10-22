cmake_minimum_required(VERSION 3.16)

project(opengl-blog CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SAMPLES "Build sample executables" ON)
option(OGL_COPY_ASSETS "Copy assets/ next to sample binaries after build" ON)
option(BUILD_CUDA "Build CUDA interop samples (future massive-mesh targets)" OFF)
option(BUILD_TESTS "Build CPU unit tests (Delaunay, procmesh, etc.)" OFF)

if(MSVC)
    add_compile_options(/utf-8)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(cmake/Dependencies.cmake)

if(BUILD_CUDA)
    include(cmake/Cuda.cmake)
endif()

add_subdirectory(framework)

if(BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
