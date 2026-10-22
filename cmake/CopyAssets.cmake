function(ogl_copy_assets target)
    if(NOT OGL_COPY_ASSETS)
        return()
    endif()
    add_custom_command(
        TARGET ${target}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/assets"
                "$<TARGET_FILE_DIR:${target}>/assets"
        COMMENT "Copying assets for ${target}"
    )
endfunction()
