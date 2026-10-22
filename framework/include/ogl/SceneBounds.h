#pragma once

#include <ogl/BoundingBox.h>

#include <glm/glm.hpp>

namespace ogl
{

class SceneBounds
{
public:
    void Reset();
    void Include(const glm::vec3& point);
    void Include(const BoundingBox& localBounds, const glm::mat4& transform = glm::mat4(1.0f));
    void Include(const BoundingBox& worldBounds);

    bool IsValid() const { return m_box.IsValid(); }
    const BoundingBox& Box() const { return m_box; }
    glm::vec3 Center() const { return m_box.Center(); }

private:
    BoundingBox m_box = BoundingBox::Empty();
};

} // namespace ogl
