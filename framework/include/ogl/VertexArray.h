#pragma once

#include <ogl/IndexBuffer.h>
#include <ogl/VertexBuffer.h>
#include <ogl/VertexLayout.h>

#include <glad/glad.h>

namespace ogl
{

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    VertexArray(VertexArray&& other) noexcept;
    VertexArray& operator=(VertexArray&& other) noexcept;

    void Bind() const;

    void SetVertexBuffer(const VertexBuffer& vbo, const VertexLayout& layout);
    void SetIndexBuffer(const IndexBuffer& ibo);

    /// Call after SetVertexBuffer. Uses @p bindingIndex as attribute location base for layout attributes.
    void SetInstanceBuffer(unsigned int bindingIndex,
                           const VertexBuffer& vbo,
                           const VertexLayout& layout,
                           unsigned int divisor = 1);

    void DrawArrays(GLenum mode, GLint first, GLsizei count) const;
    void DrawElements(GLenum mode,
                      GLsizei count,
                      GLenum indexType = GL_UNSIGNED_INT,
                      const void* offset = nullptr) const;
    void DrawElementsInstanced(GLenum mode,
                               GLsizei indexCount,
                               GLsizei instanceCount,
                               GLenum indexType = GL_UNSIGNED_INT,
                               const void* offset = nullptr) const;

    unsigned int Id() const { return m_id; }

private:
    unsigned int m_id = 0;
    const IndexBuffer* m_indexBuffer = nullptr;
};

} // namespace ogl
