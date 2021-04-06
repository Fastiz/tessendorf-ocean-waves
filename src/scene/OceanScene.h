#ifndef OPENGL_TEST_OCEANSCENE_H
#define OPENGL_TEST_OCEANSCENE_H


#include <vector>
#include <memory>
#include "../components/Scene.h"
#include "../components/Camera.h"

class OceanScene : public Scene {
private:
    Camera camera;
    std::vector<std::unique_ptr<Node>> children;
public:
    OceanScene();

    void OnUpdate(double deltaTime) override;

    void OnRender() override;

    void OnImGuiRender() override;

    void OnProcessInput(GLFWwindow* window) override;
};


#endif //OPENGL_TEST_OCEANSCENE_H
