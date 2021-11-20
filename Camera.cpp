#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up)
    : Position{ position }, Forward{ forward }, Up{ up }
{
    updateView();
}

void Camera::update(float deltaTime)
{
    updateCameraVectors();
}

void Camera::setPerspective(float fov, float aspect, float near, float far)
{
    Projection = glm::perspective(fov, aspect, near, far);
    Fov = fov;
    Aspect = aspect;
    Near = near;
    Far = far;
}

void Camera::updateView()
{
    View = glm::lookAt(Position, Position + Forward, Up);
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Forward = glm::normalize(front);

    Right = glm::normalize(glm::cross(Forward, WorldUp));
    Up = glm::normalize(glm::cross(Right, Forward));
}