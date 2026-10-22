#include <ogl/VertexBuffer.h>

namespace ogl
{

VertexBuffer::VertexBuffer(BufferUsage usage)
    : m_usage(usage)
{
    glGenBuffers(1, &m_id);
}

VertexBuffer::~VertexBuffer()
{
    if (m_id != 0)
    {
        glDeleteBuffers(1, &m_id);
        m_id = 0;
    }
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
    : m_id(other.m_id)
    , m_usage(other.m_usage)
{
    other.m_id = 0;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
    if (this != &other)
    {
        if (m_id != 0)
        {
            glDeleteBuffers(1, &m_id);
        }
        m_id = other.m_id;
        m_usage = other.m_usage;
        other.m_id = 0;
    }
    return *this;
}

void VertexBuffer::SetData(std::size_t byteSize, const void* data)
{
    Bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(byteSize), data, static_cast<GLenum>(m_usage));
}

void VertexBuffer::SetSubData(std::size_t offset, std::size_t byteSize, const void* data)
{
    Bind();
    glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(offset), static_cast<GLsizeiptr>(byteSize), data);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

} // namespace ogl
