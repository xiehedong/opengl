#pragma once

#include <ogl/BoundingBox.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ogl
{

struct PickRay
{
    glm::vec3 Origin{0.0f};
    glm::vec3 Direction{0.0f, 0.0f, -1.0f};
};

class Camera
{
public:
    glm::vec3 Target{0.0f, 0.0f, 0.0f};
    glm::vec3 WorldUp{0.0f, 1.0f, 0.0f};

    float Yaw = -90.0f;
    float Pitch = 20.0f;
    /// Unit vector from orbit target toward camera (derived from OrbitRotation).
    glm::vec3 OrbitOffsetDirection{0.0f, -0.34202015f, 0.9396926f};
    float Distance = 10.0f;
    float FovY = 45.0f;
    float NearPlane = 0.1f;
    float FarPlane = 1000.0f;

    float OrbitSensitivity = 0.3f;
    float PanSensitivity = 0.002f;
    float ZoomSensitivity = 0.12f;
    float MinDistance = 0.05f;
    float MaxDistance = 10000.0f;

    Camera();

    glm::vec3 Position() const;
    glm::vec3 Front() const;
    glm::vec3 Right() const;
    glm::vec3 Up() const;

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;

    void ProcessOrbit(float deltaYaw, float deltaPitch);
    void ProcessPan(float deltaX, float deltaY);
    void ProcessScroll(float wheelDelta);

    void SetOrbitTarget(const glm::vec3& target);
    /// Identity orbit rotation; view along world -Z (camera on +Z), Yaw=-90, Pitch=0.
    void ResetOrbitViewToNegativeZ();
    void FrameScene(const BoundingBox& sceneBounds, float aspectRatio);

    PickRay MakePickRay(float screenX, float screenY, int viewportWidth, int viewportHeight) const;

private:
    glm::quat OrbitRotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 OrbitRestOffset{0.0f, 0.0f, 1.0f};
    glm::vec3 OrbitRestRight{1.0f, 0.0f, 0.0f};

    void SyncOffsetFromRotation();
    void SyncEulerFromOrbit();
};

} // namespace ogl
