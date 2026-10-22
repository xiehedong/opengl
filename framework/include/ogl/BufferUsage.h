#pragma once

#include <glad/glad.h>

namespace ogl
{

enum class BufferUsage
{
    Static = GL_STATIC_DRAW,
    Dynamic = GL_DYNAMIC_DRAW,
    Stream = GL_STREAM_DRAW,
};

} // namespace ogl
