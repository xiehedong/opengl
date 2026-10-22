# CUDA toolchain for optional massive-mesh / GL interop samples (e.g. 30-massive-mesh).
# Future targets: add_executable(...) + target_link_libraries(... PRIVATE CUDA::cudart);
# keep CUDA targets separate from ogl_framework unless a thin wrapper is needed.

include(CheckLanguage)
check_language(CUDA)

if(NOT CMAKE_CUDA_COMPILER)
    message(FATAL_ERROR
        "BUILD_CUDA=ON but no CUDA compiler was found. "
        "Install the NVIDIA CUDA Toolkit or set BUILD_CUDA=OFF.")
endif()

enable_language(CUDA)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

find_package(CUDAToolkit REQUIRED)
