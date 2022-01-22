#include "OceanScene.h"
#include <array>
#include <glm/vec3.hpp>
#include <imgui.h>

OceanScene::OceanScene():
camera(), children(), isLineMode(false), tilingSize(1), isShowBorder(true), choppyWavesLambda(1.0f), timeScale(1.0f),
oceanScale(100.0f)
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
    std::shared_ptr<Ocean> ptr = std::make_shared<Ocean>(
            tessendorfProperties,
            material,
            tilingSize,
            isShowBorder,
            choppyWavesLambda,
            timeScale,
            oceanScale
            );
    ocean = ptr;
    children.push_back(ptr);
}

void OceanScene::OnUpdate(double deltaTime) {
    camera.OnUpdate(deltaTime);

    for(auto& node : children){
        node->OnUpdate(deltaTime);
    }
}

void OceanScene::OnRender() {
    if(isLineMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    for(auto& node : children){
        node->OnRender(camera);
    }
}

void OceanScene::OnImGuiRender() {

    OnOtherConfigGuiRender();
    OnPBRGuiRender();
    OnTessendorfGuiRender();


    for(auto& node : children){
        node->OnImGuiRender();
    }
}

void OceanScene::OnProcessInput(GLFWwindow *window) {
    camera.OnProcessInput(window);

    for(auto& node : children){
        node->OnProcessInput(window);
    }
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
        ocean->SetMaterial(material);
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
        ocean->SetChoppyWavesLambda(choppyWavesLambda);
    }

    ImGui::End();

    if(result){
        ocean->SetTessendorfProperties(tessendorfProperties);
    }
}

void OceanScene::OnOtherConfigGuiRender() {
    ImGui::Begin("Configuration");

    ImGui::Text("Tiling");
    if(ImGui::SliderInt("Size", &tilingSize, 1, 10))
        ocean->SetTiling(tilingSize);

    if(ImGui::Checkbox("Show border", &isShowBorder))
        ocean->SetShowBorder(isShowBorder);

    ImGui::Separator();
    ImGui::Text("Other");

    ImGui::Checkbox("Line mode", &isLineMode);

    if(ImGui::SliderFloat("Time scale", &timeScale, 0.001f, 100.0f))
        ocean->SetTimeScale(timeScale);

    if(ImGui::SliderFloat("Ocean scale", &oceanScale, 100.0f, 1000.0f))
        ocean->SetOceanScale(oceanScale);

    ImGui::End();

}
