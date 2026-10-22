#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ogl
{

enum class CameraMovement
{
    Forward,
    Backward,
    Left,
    Right
};

class Camera
{
public:
    glm::vec3 Position{0.0f, 0.0f, 0.0f};
    glm::vec3 Front{0.0f, 0.0f, -1.0f};
    glm::vec3 Up{0.0f, 1.0f, 0.0f};
    glm::vec3 Right{1.0f, 0.0f, 0.0f};
    glm::vec3 WorldUp{0.0f, 1.0f, 0.0f};

    float Yaw = -90.0f;
    float Pitch = 0.0f;
    float MovementSpeed = 2.5f;
    float MouseSensitivity = 0.1f;
    float Zoom = 45.0f;

    Camera() = default;
    explicit Camera(glm::vec3 position);

    glm::mat4 GetViewMatrix() const;

    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    void UpdateCameraVectors();
};

} // namespace ogl
