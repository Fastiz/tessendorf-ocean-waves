#ifndef OPENGL_TEST_CAMERA_H
#define OPENGL_TEST_CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

class Camera {
private:
    glm::mat4 view, proj;
    bool captured, firstMouse;

    float pitch, yaw;
    glm::vec3 cameraSpeed, cameraPos;
    glm::vec3 cameraFront, cameraUp;

    float lastX, lastY;
public:
    Camera();

    void OnProcessInput(GLFWwindow *window);
    void OnUpdate(double deltaTime);

    glm::vec3 GetCameraPos() const;
    glm::mat4 GetView() const;
    glm::mat4 GetProj() const;

private:
    void KeyboardInput(GLFWwindow *window);
    void MouseInput(GLFWwindow *window);
};

#endif //OPENGL_TEST_CAMERA_H
