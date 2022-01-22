#ifndef OPENGL_TEST_OCEANSCENE_H
#define OPENGL_TEST_OCEANSCENE_H


#include <vector>
#include <memory>
#include "../components/Scene.h"
#include "../components/Camera.h"
#include "Ocean.h"

class OceanScene : public Scene {
private:
    Camera camera;
    std::shared_ptr<Ocean> ocean;
    std::vector<std::shared_ptr<Node>> children;
    TessendorfProperties tessendorfProperties;
    Material material;
    bool isLineMode;
    int tilingSize;
    bool isShowBorder;
    float choppyWavesLambda;
    float timeScale;
    float oceanScale;
public:
    OceanScene();

    void OnUpdate(double deltaTime) override;

    void OnRender() override;

    void OnImGuiRender() override;

    void OnProcessInput(GLFWwindow* window) override;
private:
    void OnPBRGuiRender();
    void OnTessendorfGuiRender();
    void OnOtherConfigGuiRender();
};


#endif //OPENGL_TEST_OCEANSCENE_H
