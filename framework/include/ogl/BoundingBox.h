#pragma once

#include <glm/glm.hpp>

namespace ogl
{

class BoundingBox
{
public:
    glm::vec3 Min{1.0f};
    glm::vec3 Max{-1.0f};

    static BoundingBox Empty();
    static BoundingBox FromMinMax(const glm::vec3& min, const glm::vec3& max);
    static BoundingBox FromCenterHalfExtents(const glm::vec3& center, const glm::vec3& halfExtents);

    bool IsValid() const;
    void Expand(const glm::vec3& point);
    void Merge(const BoundingBox& other);

    glm::vec3 Center() const;
    glm::vec3 HalfExtents() const;
    glm::vec3 Size() const;
    float BoundingSphereRadius() const;
};

BoundingBox TransformAABB(const BoundingBox& local, const glm::mat4& transform);

} // namespace ogl
