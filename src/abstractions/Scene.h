#ifndef OPENGL_TEST_SCENE_H
#define OPENGL_TEST_SCENE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace abstractions {

    class Scene
    {
    public:
        Scene() = default;
        virtual ~Scene() = default;

        virtual void OnUpdate(double deltaTime) {
        }

        virtual void OnRender() {
        }

        virtual void OnImGuiRender() {
        }

        virtual void OnProcessInput(GLFWwindow* window) {

        }

    };
}

#endif //OPENGL_TEST_SCENE_H
