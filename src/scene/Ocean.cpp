#include <iostream>
#include <imgui.h>
#include "Ocean.h"
#include "../utils/grid.h"
#include "../components/Camera.h"
#include "../abstractions/RenderingShader.h"

#define VERTEX_SHADER_PATH "../../res/shaders/ocean/PBR/pbr.vertex.shader"
#define FRAGMENT_SHADER_PATH "../../res/shaders/ocean/PBR/pbr.fragment.shader"

Ocean::Ocean(int width, int height, int N, float L): width(width), height(height),
    N(N), L(L), metallic(0.5f), ao(0.5f), roughness(0.5f), lightColor(1.0f, 1.0f, 1.0f), albedo(0.1f, 0.1f, 1.0f)
{
    vao = std::make_unique<abstractions::VertexArray>();

    triangles = utils::generate_grid_mesh(width, height);

    std::vector<float> vertices_buffer = utils::generate_grid_buffer(triangles);

    vertexBuffer = std::make_unique<abstractions::VertexBuffer>(&vertices_buffer[0], vertices_buffer.size() * sizeof(float));

    layout = std::make_unique<VertexBufferLayout>();
    layout->Push<float>(3);
    vao->AddBuffer(*vertexBuffer, *layout);

    shader = std::make_unique<abstractions::RenderingShader>(
            VERTEX_SHADER_PATH,
            FRAGMENT_SHADER_PATH);
    shader->Bind();

    // VERTEX
    shader->SetUniform1i("N", N);
    shader->SetUniform1f("L", L);

    // FRAGMENT
    shader->SetUniform3f("albedo", albedo[0], albedo[1], albedo[2]);
    shader->SetUniform1f("metallic", metallic);
    shader->SetUniform1f("roughness", roughness);
    shader->SetUniform1f("ao", ao);
    shader->SetUniform3f("lightColor", lightColor[0], lightColor[1], lightColor[2]);

    spectrum_textures = textures::generate_spectrum_textures(N, L, 1.0f, 0.0f, 50.0f, L);
}

void Ocean::OnRender(Camera& camera) {
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Renderer renderer;
    renderer.Clear();

    shader->Bind();
    shader->SetUniformMat4f("view", camera.GetView());
    shader->SetUniformMat4f("proj", camera.GetProj());

    glm::vec3 cameraPos = camera.GetCameraPos();
    shader->SetUniform3f("viewPos", cameraPos[0], cameraPos[1], cameraPos[2]);

    glm::vec3 lightPos = {10.0f, 20.0f, 10.0f};
    shader->SetUniform3f("lightPosition", lightPos[0], lightPos[1], lightPos[2]);

    glm::mat4 model = glm::mat4(1.0f);
    float scale = 0.5f;
    float dx = (float)L / (float)N;
    model = glm::scale(model, glm::vec3(dx * scale, scale, dx * scale));
    shader->SetUniformMat4f("model", model);

    height_map->BindToSlot(0);
    std::get<0>(slope)->BindToSlot(1);
    std::get<1>(slope)->BindToSlot(2);

    renderer.DrawArrays(*vao, *shader, 0, width * height * 2 * 3);
}

void Ocean::OnUpdate(double deltaTime) {
    elapsedTime += deltaTime;

    textures::ssbo_pointer h_k_t = textures::generate_transform_texture(std::get<0>(spectrum_textures), std::get<1>(spectrum_textures), N, L, (float) elapsedTime);
    height_map = textures::update_fft_texture(h_k_t, N);
    slope = textures::update_slope_texture(height_map, N, L);
}

void Ocean::OnImGuiRender() {
    ImGui::Begin("PBR Parameters");

    bool result;

    shader->Bind();

    result = ImGui::ColorEdit3("Albedo", reinterpret_cast<float *>(&albedo));
    if(result){
        shader->SetUniform3f("albedo", albedo[0], albedo[1], albedo[2]);
    }

    result = ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
    if(result){
        shader->SetUniform1f("metallic", metallic);
    }

    result = ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
    if(result){
        shader->SetUniform1f("roughness", roughness);
    }

    result = ImGui::SliderFloat("Ambient occlusion", &ao, 0.0f, 1.0f);
    if(result){
        shader->SetUniform1f("ao", ao);
    }

    result = ImGui::ColorEdit3("Light color", reinterpret_cast<float *>(&lightColor));
    if(result){
        shader->SetUniform3f("lightColor", lightColor[0], lightColor[1], lightColor[2]);
    }

    ImGui::End();
}
