#pragma once

#include <glad/glad.h>

#include <cstddef>
#include <vector>

namespace ogl
{

struct VertexAttribute
{
    unsigned int index = 0;
    int componentCount = 0;
    GLenum type = GL_FLOAT;
    GLboolean normalized = GL_FALSE;
    std::size_t offset = 0;
};

class VertexLayout
{
public:
    static VertexLayout Position3();
    static VertexLayout Position3TexCoord2();

    std::size_t Stride() const { return m_stride; }
    const std::vector<VertexAttribute>& Attributes() const { return m_attributes; }

    /// Call with VAO and GL_ARRAY_BUFFER already bound.
    void Apply() const;

private:
    std::size_t m_stride = 0;
    std::vector<VertexAttribute> m_attributes;
};

} // namespace ogl
