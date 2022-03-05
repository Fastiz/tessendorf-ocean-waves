#include "OceanScene.h"
#include "OceanGerstner.h"
#include <array>
#include <glm/vec3.hpp>
#include <imgui.h>

OceanScene::OceanScene():
camera(), isLineMode(false), tilingSize(1), isShowBorder(true), choppyWavesLambda(1.0f), timeScale(1.0f),
oceanScale(100.0f), selectedMethod(Gerstner), gerstnerNumberOfWaves(78)
{
    GLCall(glEnable(GL_DEPTH_TEST))
    GLCall(glEnable(GL_BLEND))
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

    this->tessendorfProperties = {
            .N = 256,
            .L = 1000,
            .A = 500.0f,
            .windDirection = {1.0f, 0.0f},
            .windSpeed = 50.0f,
    };

    this->material = {
            .metallic = 0.5f,
            .ao = 0.5f,
            .roughness = 0.5f,
            .lightColor = { 1.0f, 1.0f, 1.0f },
            .albedo = { 0.0f, 0.0f, 1.0f },
            .lightPosition = {0.0f, 160.0f, 0.0f},
            .lightAttenuationScale = 750.0f
    };

    std::shared_ptr<OceanFFT> tessendorf_ocean_ptr = std::make_shared<OceanFFT>(
            tessendorfProperties,
            material,
            tilingSize,
            isShowBorder,
            choppyWavesLambda,
            timeScale,
            oceanScale
            );
    
    this->gerstnerWaveConstaints = {
            .minAmplitude = 0.0, .maxAmplitude = 0.249,
            .minFrequency = 0.0, .maxFrequency = 0.321,
            .minPhase = 0.0, .maxPhase = 0.264,
    };

    tessendorf_ocean = tessendorf_ocean_ptr;

    std::shared_ptr<OceanGerstner> gerstner_ocean_ptr = std::make_shared<OceanGerstner>(
            tessendorfProperties.N,
            material,
            gerstnerWaveConstaints
            );

    gerstner_ocean = gerstner_ocean_ptr;
}

void OceanScene::OnUpdate(double deltaTime) {
    camera.OnUpdate(deltaTime);

    if(selectedMethod == Tessendorf)
        tessendorf_ocean->OnUpdate(deltaTime);
    else
        gerstner_ocean->OnUpdate(deltaTime);
}

void OceanScene::OnRender() {
    if(isLineMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if(selectedMethod == Tessendorf)
        tessendorf_ocean->OnRender(camera);
    else
        gerstner_ocean->OnRender(camera);
}

void OceanScene::OnImGuiRender() {
    OnOtherConfigGuiRender();
    OnPBRGuiRender();

    if(selectedMethod == Tessendorf)
        OnTessendorfGuiRender();
    else
        OnGerstnerGuiRender();
}

void OceanScene::OnProcessInput(GLFWwindow *window) {
    camera.OnProcessInput(window);
}

void OceanScene::OnPBRGuiRender() {
    ImGui::Begin("PBR Parameters");

    bool result = false;

    ImGui::Text("Material");
    result = ImGui::ColorEdit3("Albedo", reinterpret_cast<float *>(&material.albedo)) || result;
    result = ImGui::SliderFloat("Metallic", &material.metallic, 0.0f, 1.0f) || result;
    result = ImGui::SliderFloat("Roughness", &material.roughness, 0.0f, 1.0f) || result;
    result = ImGui::SliderFloat("Ambient occlusion", &material.ao, 0.0f, 1.0f) || result;

    ImGui::Separator();
    ImGui::Text("Light");
    result = ImGui::ColorEdit3("Color", reinterpret_cast<float *>(&material.lightColor)) || result;
    result = ImGui::SliderFloat3("Position", reinterpret_cast<float *>(&material.lightPosition), 0.0f, 1000.0f) || result;
    result = ImGui::SliderFloat("Attenuation scale", &material.lightAttenuationScale, 1.0f, 10000.0f) || result;

    ImGui::End();

    if(result){
        tessendorf_ocean->SetMaterial(material);
        gerstner_ocean->SetMaterial(material);
    }
}

void OceanScene::OnTessendorfGuiRender() {
    ImGui::Begin("Tessendorf Parameters");

    bool result;

    int power = std::log2(tessendorfProperties.N);
    result = ImGui::SliderInt("N", &power, 6, 10);
    if(result)
        tessendorfProperties.N = 1 << power;

    result = ImGui::SliderFloat("L", &tessendorfProperties.L, 256, 16384) || result;
    result = ImGui::SliderFloat("A", &tessendorfProperties.A, 1.0f, 1000.0f) || result;
    result = ImGui::SliderFloat2("Wind direction", (float*) &tessendorfProperties.windDirection, -1.0f, 1.0f) || result;
    result = ImGui::SliderFloat("Wind speed", &tessendorfProperties.windSpeed, 1.0f, 1000.0f) || result;

    if(ImGui::SliderFloat("Displacement lambda", &choppyWavesLambda, 0.0f, 10.0f)){
        tessendorf_ocean->SetChoppyWavesLambda(choppyWavesLambda);
    }

    ImGui::End();

    if(result){
        tessendorf_ocean->SetTessendorfProperties(tessendorfProperties);
    }
}

void OceanScene::OnOtherConfigGuiRender() {
    ImGui::Begin("Configuration");

    ImGui::Text("Method");
    static const char* method_labels[]{"Tessendorf", "Gerstner"};
    ImGui::ListBox("Method", (int*) &selectedMethod, method_labels, 2, 1);

    ImGui::Separator();
    ImGui::Text("Tiling");
    if(ImGui::SliderInt("Size", &tilingSize, 1, 10))
        tessendorf_ocean->SetTiling(tilingSize);

    if(ImGui::Checkbox("Show border", &isShowBorder))
        tessendorf_ocean->SetShowBorder(isShowBorder);

    ImGui::Separator();
    ImGui::Text("Other");

    ImGui::Checkbox("Line mode", &isLineMode);

    if(ImGui::SliderFloat("Time scale", &timeScale, 0.001f, 100.0f)) {
        tessendorf_ocean->SetTimeScale(timeScale);
        gerstner_ocean->SetTimeScale(timeScale);
    }
    if(ImGui::SliderFloat("OceanFFT scale", &oceanScale, 100.0f, 1000.0f))
        tessendorf_ocean->SetOceanScale(oceanScale);

    ImGui::End();

}

void OceanScene::OnGerstnerGuiRender() {
    ImGui::Begin("Gerstner Parameters");

    bool result = false;

    ImGui::Text("Amplitude range");
    result = ImGui::SliderFloat("min##Amplitude", &gerstnerWaveConstaints.minAmplitude, 0.0, gerstnerWaveConstaints.maxAmplitude) || result;
    result = ImGui::SliderFloat("max##Amplitude", &gerstnerWaveConstaints.maxAmplitude, gerstnerWaveConstaints.minAmplitude, 1.0) || result;
    
    ImGui::Separator();
    ImGui::Text("Frequency range");
    result = ImGui::SliderFloat("min##Frequency", &gerstnerWaveConstaints.minFrequency, 0.0, gerstnerWaveConstaints.maxFrequency) || result;
    result = ImGui::SliderFloat("max##Frequency", &gerstnerWaveConstaints.maxFrequency, gerstnerWaveConstaints.minFrequency, 1.0) || result;
    
    ImGui::Separator();
    ImGui::Text("Phase range");
    result = ImGui::SliderFloat("min##Phase", &gerstnerWaveConstaints.minPhase, 0.0, gerstnerWaveConstaints.maxPhase) || result;

    const static float PI = 3.14;
    result = ImGui::SliderFloat("max##Phase", &gerstnerWaveConstaints.maxPhase, gerstnerWaveConstaints.minPhase, 2 * PI) || result;

    ImGui::Separator();
    ImGui::Text("Number of waves");
    if(ImGui::SliderInt("##NumberOfWaves", (int* ) &gerstnerNumberOfWaves, 1, 1000))
        gerstner_ocean->SetNumberOfWaves(gerstnerNumberOfWaves);

    if(result)
        gerstner_ocean->SetWaveConstraints(gerstnerWaveConstaints);
    
    ImGui::End();
}
