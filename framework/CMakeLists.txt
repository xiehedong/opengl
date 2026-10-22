set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

set(OGL_FRAMEWORK_SOURCES
    src/Application.cpp
    src/Camera.cpp
    src/FileSystem.cpp
    src/ImGuiLayer.cpp
    src/Shader.cpp
    src/Texture2D.cpp
    src/Window.cpp
    src/glad.c
    src/stb_image.cpp
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(ogl_framework STATIC ${OGL_FRAMEWORK_SOURCES})

target_include_directories(ogl_framework
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(ogl_framework
    PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_GLAD
    PRIVATE
        OGL_PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}"
)

target_link_directories(ogl_framework PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(ogl_framework PUBLIC glfw3)

if(WIN32)
    target_link_libraries(ogl_framework PUBLIC opengl32)
endif()

if(MSVC)
    target_compile_options(ogl_framework PRIVATE $<$<CONFIG:Debug>:/ZI>)
    target_link_options(ogl_framework PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
endif()
