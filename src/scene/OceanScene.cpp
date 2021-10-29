#include "OceanScene.h"
#include <array>
#include <glm/vec3.hpp>
#include <imgui.h>

OceanScene::OceanScene(): camera(), children(), isLineMode(false) {
    GLCall(glEnable(GL_DEPTH_TEST))
    GLCall(glEnable(GL_BLEND))
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

    this->tessendorfProperties = { .width = 1024, .height = 1024, .N = 512, .L = 1000 };
    this->material = {
            .metallic = 0.5f,
            .ao = 0.5f,
            .roughness = 0.5f,
            .lightColor = { 1.0f, 1.0f, 1.0f },
            .albedo = { 0.0f, 0.0f, 1.0f }
    };
    std::shared_ptr<Ocean> ptr = std::make_shared<Ocean>(tessendorfProperties, material);
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
    ImGui::Begin("Configuration");

    OnOtherConfigGuiRender();
    OnPBRGuiRender();
    OnTessendorfGuiRender();

    ImGui::End();

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

    result = ImGui::ColorEdit3("Albedo", reinterpret_cast<float *>(&material.albedo)) || result;
    result = ImGui::SliderFloat("Metallic", &material.metallic, 0.0f, 1.0f) || result;
    result = ImGui::SliderFloat("Roughness", &material.roughness, 0.0f, 1.0f) || result;
    result = ImGui::SliderFloat("Ambient occlusion", &material.ao, 0.0f, 1.0f) || result;
    result = ImGui::ColorEdit3("Light color", reinterpret_cast<float *>(&material.lightColor)) || result;

    ImGui::End();

    if(result){
        ocean->SetMaterial(material);
    }
}

void OceanScene::OnTessendorfGuiRender() {
    ImGui::Begin("Tessendorf Parameters");

    bool result = false;

    int power = std::log2(tessendorfProperties.N);
    bool resultN = ImGui::SliderInt("N", &power, 8, 10);
    if(resultN)
        tessendorfProperties.N = 1 << power;
    result = resultN || result;
    result = ImGui::SliderFloat("L", &tessendorfProperties.L, 256, 2048) || result;
    result = ImGui::SliderInt("width", &tessendorfProperties.width, 256, 2024) || result;
    result = ImGui::SliderInt("height", &tessendorfProperties.height, 256, 2024) || result;

    ImGui::End();

    if(result){
        ocean->SetTessendorfProperties(tessendorfProperties);
    }
}

void OceanScene::OnOtherConfigGuiRender() {
    ImGui::Begin("Other Parameters");

    bool modified = ImGui::Checkbox("Line mode", &isLineMode);

    ImGui::End();

}
