#include <ogl/VertexArray.h>

namespace ogl
{

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
    if (m_id != 0)
    {
        glDeleteVertexArrays(1, &m_id);
        m_id = 0;
    }
}

VertexArray::VertexArray(VertexArray&& other) noexcept
    : m_id(other.m_id)
    , m_indexBuffer(other.m_indexBuffer)
{
    other.m_id = 0;
    other.m_indexBuffer = nullptr;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
    if (this != &other)
    {
        if (m_id != 0)
        {
            glDeleteVertexArrays(1, &m_id);
        }
        m_id = other.m_id;
        m_indexBuffer = other.m_indexBuffer;
        other.m_id = 0;
        other.m_indexBuffer = nullptr;
    }
    return *this;
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::SetVertexBuffer(const VertexBuffer& vbo, const VertexLayout& layout)
{
    Bind();
    vbo.Bind();
    layout.Apply();
}

void VertexArray::SetIndexBuffer(const IndexBuffer& ibo)
{
    Bind();
    ibo.Bind();
    m_indexBuffer = &ibo;
}

void VertexArray::SetInstanceBuffer(unsigned int bindingIndex,
                                    const VertexBuffer& vbo,
                                    const VertexLayout& layout,
                                    unsigned int divisor)
{
    Bind();
    vbo.Bind();
    for (const VertexAttribute& attr : layout.Attributes())
    {
        const unsigned int location = bindingIndex + attr.index;
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(
            location,
            attr.componentCount,
            attr.type,
            attr.normalized,
            static_cast<GLsizei>(layout.Stride()),
            reinterpret_cast<const void*>(attr.offset));
        glVertexAttribDivisor(location, divisor);
    }
}

void VertexArray::DrawArrays(GLenum mode, GLint first, GLsizei count) const
{
    Bind();
    glDrawArrays(mode, first, count);
}

void VertexArray::DrawElements(GLenum mode, GLsizei count, GLenum indexType, const void* offset) const
{
    Bind();
    const GLenum type = m_indexBuffer != nullptr ? m_indexBuffer->IndexType() : indexType;
    glDrawElements(mode, count, type, offset);
}

void VertexArray::DrawElementsInstanced(GLenum mode,
                                        GLsizei indexCount,
                                        GLsizei instanceCount,
                                        GLenum indexType,
                                        const void* offset) const
{
    Bind();
    const GLenum type = m_indexBuffer != nullptr ? m_indexBuffer->IndexType() : indexType;
    glDrawElementsInstanced(mode, indexCount, type, offset, instanceCount);
}

} // namespace ogl
