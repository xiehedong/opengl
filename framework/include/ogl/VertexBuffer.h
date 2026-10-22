#pragma once

#include <ogl/BufferUsage.h>

#include <glad/glad.h>

#include <cstddef>

namespace ogl
{

class VertexBuffer
{
public:
    explicit VertexBuffer(BufferUsage usage = BufferUsage::Static);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept;
    VertexBuffer& operator=(VertexBuffer&& other) noexcept;

    void SetData(std::size_t byteSize, const void* data);
    void SetSubData(std::size_t offset, std::size_t byteSize, const void* data);

    void Bind() const;
    unsigned int Id() const { return m_id; }

private:
    unsigned int m_id = 0;
    BufferUsage m_usage = BufferUsage::Static;
};

} // namespace ogl
