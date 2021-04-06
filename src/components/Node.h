#ifndef OPENGL_TEST_NODE_H
#define OPENGL_TEST_NODE_H

class Camera;
class GLFWwindow;

class Node
{
public:
    Node() = default;
    virtual ~Node() = default;

    virtual void OnUpdate(double deltaTime) {
    }

    virtual void OnRender(Camera& camera) {
    }

    virtual void OnImGuiRender() {
    }

    virtual void OnProcessInput(GLFWwindow* window) {

    }

};

#endif //OPENGL_TEST_NODE_H
