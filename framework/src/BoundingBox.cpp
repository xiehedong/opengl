#include <ogl/BoundingBox.h>

#include <algorithm>
#include <cmath>
#include <limits>

namespace ogl
{

BoundingBox BoundingBox::Empty()
{
    BoundingBox box;
    box.Min = glm::vec3(std::numeric_limits<float>::max());
    box.Max = glm::vec3(std::numeric_limits<float>::lowest());
    return box;
}

BoundingBox BoundingBox::FromMinMax(const glm::vec3& min, const glm::vec3& max)
{
    BoundingBox box;
    box.Min = min;
    box.Max = max;
    return box;
}

BoundingBox BoundingBox::FromCenterHalfExtents(const glm::vec3& center, const glm::vec3& halfExtents)
{
    return FromMinMax(center - halfExtents, center + halfExtents);
}

bool BoundingBox::IsValid() const
{
    return Min.x <= Max.x && Min.y <= Max.y && Min.z <= Max.z;
}

void BoundingBox::Expand(const glm::vec3& point)
{
    Min = glm::min(Min, point);
    Max = glm::max(Max, point);
}

void BoundingBox::Merge(const BoundingBox& other)
{
    if (!other.IsValid())
    {
        return;
    }
    if (!IsValid())
    {
        Min = other.Min;
        Max = other.Max;
        return;
    }
    Min = glm::min(Min, other.Min);
    Max = glm::max(Max, other.Max);
}

glm::vec3 BoundingBox::Center() const
{
    return (Min + Max) * 0.5f;
}

glm::vec3 BoundingBox::HalfExtents() const
{
    return (Max - Min) * 0.5f;
}

glm::vec3 BoundingBox::Size() const
{
    return Max - Min;
}

float BoundingBox::BoundingSphereRadius() const
{
    return glm::length(HalfExtents());
}

BoundingBox TransformAABB(const BoundingBox& local, const glm::mat4& transform)
{
    if (!local.IsValid())
    {
        return BoundingBox::Empty();
    }

    const glm::vec3 corners[8] = {
        {local.Min.x, local.Min.y, local.Min.z},
        {local.Max.x, local.Min.y, local.Min.z},
        {local.Max.x, local.Max.y, local.Min.z},
        {local.Min.x, local.Max.y, local.Min.z},
        {local.Min.x, local.Min.y, local.Max.z},
        {local.Max.x, local.Min.y, local.Max.z},
        {local.Max.x, local.Max.y, local.Max.z},
        {local.Min.x, local.Max.y, local.Max.z},
    };

    BoundingBox world = BoundingBox::Empty();
    for (const glm::vec3& corner : corners)
    {
        const glm::vec4 transformed = transform * glm::vec4(corner, 1.0f);
        world.Expand(glm::vec3(transformed));
    }
    return world;
}

} // namespace ogl
