#pragma once

#include <ogl/BufferUsage.h>

#include <glad/glad.h>

#include <cstddef>

namespace ogl
{

class IndexBuffer
{
public:
    explicit IndexBuffer(BufferUsage usage = BufferUsage::Static);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    IndexBuffer(IndexBuffer&& other) noexcept;
    IndexBuffer& operator=(IndexBuffer&& other) noexcept;

    void SetData(std::size_t byteSize, const void* data);
    void SetSubData(std::size_t offset, std::size_t byteSize, const void* data);

    void Bind() const;
    unsigned int Id() const { return m_id; }

    GLenum IndexType() const { return m_indexType; }
    void SetIndexType(GLenum indexType) { m_indexType = indexType; }

private:
    unsigned int m_id = 0;
    BufferUsage m_usage = BufferUsage::Static;
    GLenum m_indexType = GL_UNSIGNED_INT;
};

} // namespace ogl
