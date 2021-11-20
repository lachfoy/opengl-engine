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
    float Yaw = -90.0f;
    float Pitch = 0.0f;
    
    // camera options
    float Fov;
    float Aspect;
    float Near;
    float Far;

    Camera();
    Camera(glm::vec3 position, glm::vec3 forward, glm::vec3 up);

    void update(float deltaTime);
    void setPerspective(float fov, float aspect, float near, float far);

    void updateView();

private:
    void updateCameraVectors();
};