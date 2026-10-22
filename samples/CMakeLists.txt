include(CopyAssets)

function(ogl_add_sample target_name)
    add_executable(${target_name} ${ARGN})
    target_link_libraries(${target_name} PRIVATE ogl_framework)
    if(MSVC)
        target_compile_options(${target_name} PRIVATE /utf-8)
    endif()
    ogl_copy_assets(${target_name})
endfunction()

add_subdirectory(01-hello-triangle)
add_subdirectory(02-textured-cubes)
