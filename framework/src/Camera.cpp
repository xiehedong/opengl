#include <ogl/Camera.h>

namespace ogl
{

Camera::Camera(glm::vec3 position)
    : Position(position)
{
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    const float velocity = MovementSpeed * deltaTime;
    if (direction == CameraMovement::Forward)
    {
        Position += Front * velocity;
    }
    if (direction == CameraMovement::Backward)
    {
        Position -= Front * velocity;
    }
    if (direction == CameraMovement::Left)
    {
        Position -= Right * velocity;
    }
    if (direction == CameraMovement::Right)
    {
        Position += Right * velocity;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
        {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f)
        {
            Pitch = -89.0f;
        }
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= yoffset;
    if (Zoom < 1.0f)
    {
        Zoom = 1.0f;
    }
    if (Zoom > 100.0f)
    {
        Zoom = 100.0f;
    }
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

} // namespace ogl
