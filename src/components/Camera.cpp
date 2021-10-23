#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <iostream>

Camera::Camera()
    :
    view(
        glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
        )
    ),
    proj(
        glm::perspective(glm::radians(45.f), 800.0f / 600.0f, 0.1f, 1000.0f)
    ),
    captured(false),
    firstMouse(true),
    cameraSpeed(0.0f, 0.0f, 0.0f), cameraUp(0.0f, 1.0f, 0.0f),
    cameraFront(0.0f, 0.0f, -1.0f), cameraPos(0.0f, 0.0f, 3.0f),
    lastX(400), lastY(300), yaw(-90.0f), pitch(0.0f)
{

}

void Camera::OnUpdate(double deltaTime) {
    cameraPos += cameraSpeed * glm::vec1(deltaTime);

    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::GetView() const {
    return view;
}

glm::mat4 Camera::GetProj() const {
    return proj;
}

void Camera::OnProcessInput(GLFWwindow *window) {
    Camera::KeyboardInput(window);

    if(captured)
        Camera::MouseInput(window);
}

void Camera::KeyboardInput(GLFWwindow *window) {
    if(captured)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        captured = false;
    if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        captured = true;

    cameraSpeed = glm::vec3(0.0f);
    float cameraSpeedMagnitude = 4.0f; // adjust accordingly
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeedMagnitude *= 2;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraSpeed += cameraSpeedMagnitude * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraSpeed -= cameraSpeedMagnitude * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraSpeed -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
                       cameraSpeedMagnitude;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraSpeed += glm::normalize(glm::cross(cameraFront, cameraUp)) *
                       cameraSpeedMagnitude;
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraSpeed += glm::vec3(0.0f, 1.0f, 0.0f) *
                       cameraSpeedMagnitude;
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraSpeed += glm::vec3(0.0f, -1.0f, 0.0f) *
                       cameraSpeedMagnitude;
}

void Camera::MouseInput(GLFWwindow *window) {
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    if(firstMouse){
        firstMouse = false;
        lastX = (float) xPos;
        lastY = (float) yPos;
    }

    float xoffset = (float) xPos - lastX;
    float yoffset = lastY - (float) yPos;
    lastX = (float) xPos;
    lastY = (float) yPos;

    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) *  cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
}

glm::vec3 Camera::GetCameraPos() const {
    return cameraPos;
}
