#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 6.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 75.0f;

class Camera
{
public:
    glm::mat4 Projection;
    glm::mat4 View;

    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Forward = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 WorldUp;
    
    // euler Angles
    float Yaw;
    float Pitch;
    
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Fov;
    float Aspect;
    float Near;
    float Far;

    Camera();
    Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up);

    void update(float deltaTime);
    void setPerspective(float fov, float aspect, float near, float far);

    void updateView();

    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);

private:
    void updateCameraVectors();
};