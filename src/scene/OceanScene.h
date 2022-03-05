#ifndef OPENGL_TEST_OCEANSCENE_H
#define OPENGL_TEST_OCEANSCENE_H


#include <vector>
#include <memory>
#include "../components/Scene.h"
#include "../components/Camera.h"
#include "OceanFFT.h"
#include "OceanGerstner.h"

enum OceanMethod {
    Tessendorf, Gerstner
};

class OceanScene : public Scene {
private:
    Camera camera;
    std::shared_ptr<OceanFFT> tessendorf_ocean;
    std::shared_ptr<OceanGerstner> gerstner_ocean;

    WaveConstraints gerstnerWaveConstaints;
    TessendorfProperties tessendorfProperties;
    Material material;
    bool isLineMode;
    int tilingSize;
    bool isShowBorder;
    float choppyWavesLambda;
    float timeScale;
    float oceanScale;
    unsigned int gerstnerNumberOfWaves;

    OceanMethod selectedMethod;
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
    void OnGerstnerGuiRender();
};


#endif //OPENGL_TEST_OCEANSCENE_H
