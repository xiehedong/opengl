#include <ogl/IndexBuffer.h>

namespace ogl
{

IndexBuffer::IndexBuffer(BufferUsage usage)
    : m_usage(usage)
{
    glGenBuffers(1, &m_id);
}

IndexBuffer::~IndexBuffer()
{
    if (m_id != 0)
    {
        glDeleteBuffers(1, &m_id);
        m_id = 0;
    }
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
    : m_id(other.m_id)
    , m_usage(other.m_usage)
    , m_indexType(other.m_indexType)
{
    other.m_id = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
    if (this != &other)
    {
        if (m_id != 0)
        {
            glDeleteBuffers(1, &m_id);
        }
        m_id = other.m_id;
        m_usage = other.m_usage;
        m_indexType = other.m_indexType;
        other.m_id = 0;
    }
    return *this;
}

void IndexBuffer::SetData(std::size_t byteSize, const void* data)
{
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(byteSize), data, static_cast<GLenum>(m_usage));
}

void IndexBuffer::SetSubData(std::size_t offset, std::size_t byteSize, const void* data)
{
    Bind();
    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLintptr>(offset),
        static_cast<GLsizeiptr>(byteSize),
        data);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

} // namespace ogl
