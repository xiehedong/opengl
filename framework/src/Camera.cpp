#include <ogl/Camera.h>

#include <algorithm>
#include <cmath>

#include <glm/gtc/quaternion.hpp>

namespace ogl
{

namespace
{

glm::vec3 FrontFromEuler(float yaw, float pitch)
{
    glm::vec3 front;
    front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    front.y = std::sin(glm::radians(pitch));
    front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
    return glm::normalize(front);
}

glm::vec3 RotateByQuat(const glm::quat& rotation, const glm::vec3& vector)
{
    return glm::mat3_cast(rotation) * vector;
}

} // namespace

Camera::Camera()
{
    OrbitRestOffset = glm::normalize(-FrontFromEuler(Yaw, Pitch));
    glm::vec3 restRight = glm::cross(WorldUp, OrbitRestOffset);
    if (glm::dot(restRight, restRight) < 1e-8f)
    {
        restRight = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    OrbitRestRight = glm::normalize(restRight);
    OrbitRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    SyncOffsetFromRotation();
}

void Camera::SyncOffsetFromRotation()
{
    OrbitOffsetDirection = glm::normalize(RotateByQuat(OrbitRotation, OrbitRestOffset));
}

glm::vec3 Camera::Front() const
{
    return glm::normalize(-OrbitOffsetDirection);
}

glm::vec3 Camera::Position() const
{
    return Target + OrbitOffsetDirection * Distance;
}

glm::vec3 Camera::Right() const
{
    return glm::normalize(RotateByQuat(OrbitRotation, OrbitRestRight));
}

glm::vec3 Camera::Up() const
{
    const glm::vec3 viewForward = Front();
    const glm::vec3 right = Right();
    return glm::normalize(glm::cross(right, viewForward));
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
    // Mouse right (+deltaYaw): scene CCW about +WorldUp when viewed from above (RH; camera path is CW).
    const float dy = -deltaYaw * OrbitSensitivity;
    const float dp = deltaPitch * OrbitSensitivity;

    if (std::abs(dy) > 1e-6f)
    {
        const glm::quat qYaw = glm::angleAxis(glm::radians(dy), glm::normalize(WorldUp));
        OrbitRotation = glm::normalize(qYaw * OrbitRotation);
    }

    if (std::abs(dp) > 1e-6f)
    {
        const glm::quat qPitch = glm::angleAxis(glm::radians(dp), OrbitRestRight);
        OrbitRotation = glm::normalize(OrbitRotation * qPitch);
    }

    SyncOffsetFromRotation();
    SyncEulerFromOrbit();
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

void Camera::ResetOrbitViewToNegativeZ()
{
    Yaw = -90.0f;
    Pitch = 0.0f;
    OrbitRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    OrbitRestOffset = glm::vec3(0.0f, 0.0f, 1.0f);
    OrbitRestRight = glm::vec3(1.0f, 0.0f, 0.0f);
    SyncOffsetFromRotation();
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

void Camera::SyncEulerFromOrbit()
{
    const glm::vec3 front = Front();
    Pitch = glm::degrees(std::asin(glm::clamp(front.y, -1.0f, 1.0f)));
    Yaw = glm::degrees(std::atan2(front.z, front.x));
}

} // namespace ogl
