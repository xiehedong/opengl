#include <ogl/SceneBounds.h>

namespace ogl
{

void SceneBounds::Reset()
{
    m_box = BoundingBox::Empty();
}

void SceneBounds::Include(const glm::vec3& point)
{
    m_box.Expand(point);
}

void SceneBounds::Include(const BoundingBox& localBounds, const glm::mat4& transform)
{
    m_box.Merge(TransformAABB(localBounds, transform));
}

void SceneBounds::Include(const BoundingBox& worldBounds)
{
    m_box.Merge(worldBounds);
}

} // namespace ogl
