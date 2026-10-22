#include <ogl/VertexLayout.h>

namespace ogl
{

VertexLayout VertexLayout::Position3()
{
    VertexLayout layout;
    layout.m_stride = 3 * sizeof(float);
    layout.m_attributes.push_back({0, 3, GL_FLOAT, GL_FALSE, 0});
    return layout;
}

VertexLayout VertexLayout::Position3TexCoord2()
{
    VertexLayout layout;
    layout.m_stride = 5 * sizeof(float);
    layout.m_attributes.push_back({0, 3, GL_FLOAT, GL_FALSE, 0});
    layout.m_attributes.push_back({1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float)});
    return layout;
}

void VertexLayout::Apply() const
{
    for (const VertexAttribute& attr : m_attributes)
    {
        glEnableVertexAttribArray(attr.index);
        glVertexAttribPointer(
            attr.index,
            attr.componentCount,
            attr.type,
            attr.normalized,
            static_cast<GLsizei>(m_stride),
            reinterpret_cast<const void*>(attr.offset));
    }
}

} // namespace ogl
