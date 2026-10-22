#include <ogl/Camera.h>

#include <algorithm>
#include <cmath>

namespace ogl
{

glm::vec3 Camera::Front() const
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    return glm::normalize(front);
}

glm::vec3 Camera::Position() const
{
    return Target - Front() * Distance;
}

glm::vec3 Camera::Right() const
{
    return glm::normalize(glm::cross(Front(), WorldUp));
}

glm::vec3 Camera::Up() const
{
    return glm::normalize(glm::cross(Right(), Front()));
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position(), Target, Up());
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(FovY), aspectRatio, NearPlane, FarPlane);
}

void Camera::ProcessOrbit(float deltaYaw, float deltaPitch)
{
    Yaw += deltaYaw * OrbitSensitivity;
    Pitch += deltaPitch * OrbitSensitivity;
    ClampPitch();
}

void Camera::ProcessPan(float deltaX, float deltaY)
{
    const float panScale = PanSensitivity * Distance;
    const glm::vec3 right = Right();
    const glm::vec3 up = Up();
    Target -= right * (deltaX * panScale);
    Target -= up * (deltaY * panScale);
}

void Camera::ProcessScroll(float wheelDelta)
{
    const float factor = 1.0f - wheelDelta * ZoomSensitivity;
    Distance *= factor;
    Distance = std::clamp(Distance, MinDistance, MaxDistance);
}

void Camera::SetOrbitTarget(const glm::vec3& target)
{
    Target = target;
}

void Camera::FrameScene(const BoundingBox& sceneBounds, float aspectRatio)
{
    if (!sceneBounds.IsValid())
    {
        return;
    }

    Target = sceneBounds.Center();
    const float radius = std::max(sceneBounds.BoundingSphereRadius(), 0.001f);
    const float verticalFov = glm::radians(FovY);
    const float horizontalFov = 2.0f * std::atan(std::tan(verticalFov * 0.5f) * std::max(aspectRatio, 0.01f));
    const float limitingFov = std::min(verticalFov, horizontalFov);
    const float sinHalfFov = std::sin(limitingFov * 0.5f);
    Distance = sinHalfFov > 0.0001f ? radius / sinHalfFov : radius;
    Distance = std::clamp(Distance * 1.15f, MinDistance, MaxDistance);

    const float span = glm::length(sceneBounds.Size());
    NearPlane = std::max(0.001f, Distance * 0.001f);
    FarPlane = std::max(Distance + span * 4.0f, NearPlane + 1.0f);
}

PickRay Camera::MakePickRay(float screenX, float screenY, int viewportWidth, int viewportHeight) const
{
    PickRay ray;
    if (viewportWidth <= 0 || viewportHeight <= 0)
    {
        ray.Origin = Position();
        ray.Direction = Front();
        return ray;
    }

    const float aspect = static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight);
    const glm::mat4 projection = GetProjectionMatrix(aspect);
    const glm::mat4 view = GetViewMatrix();
    const glm::mat4 invViewProj = glm::inverse(projection * view);

    const float ndcX = (2.0f * screenX) / static_cast<float>(viewportWidth) - 1.0f;
    const float ndcY = 1.0f - (2.0f * screenY) / static_cast<float>(viewportHeight);

    const glm::vec4 nearPoint = invViewProj * glm::vec4(ndcX, ndcY, -1.0f, 1.0f);
    const glm::vec4 farPoint = invViewProj * glm::vec4(ndcX, ndcY, 1.0f, 1.0f);

    glm::vec3 nearWorld = glm::vec3(nearPoint) / nearPoint.w;
    glm::vec3 farWorld = glm::vec3(farPoint) / farPoint.w;

    ray.Origin = nearWorld;
    ray.Direction = glm::normalize(farWorld - nearWorld);
    return ray;
}

void Camera::ClampPitch()
{
    Pitch = std::clamp(Pitch, -89.0f, 89.0f);
}

} // namespace ogl
