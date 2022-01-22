#include <iostream>
#include <imgui.h>
#include "Ocean.h"
#include "../utils/grid.h"
#include "../components/Camera.h"
#include "../abstractions/RenderingShader.h"

#define VERTEX_SHADER_PATH      "../../res/shaders/ocean/PBR/pbr.vertex.shader"
#define FRAGMENT_SHADER_PATH    "../../res/shaders/ocean/PBR/pbr.fragment.shader"

Ocean::Ocean(TessendorfProperties tessendorfProperties, Material material, unsigned int tilingSize, bool isShowBorder)
    : tessendorfProperties(tessendorfProperties), material(material), tilingSize(tilingSize), showBorder(isShowBorder)
{
    initializeVertexBuffer();
    initializePBRShader();
    initializeSpectrumTextures();
}

void Ocean::OnRender(Camera& camera) {
    Renderer renderer;
    renderer.Clear();

    shader->Bind();
    shader->SetUniformMat4f("view", camera.GetView());
    shader->SetUniformMat4f("proj", camera.GetProj());

    glm::vec3 cameraPos = camera.GetCameraPos();
    shader->SetUniform3f("viewPos", cameraPos[0], cameraPos[1], cameraPos[2]);

    glm::vec3 lightPos = {0.0f, 10000.0f, 0.0f};
    shader->SetUniform3f("lightPosition", lightPos[0], lightPos[1], lightPos[2]);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.2f));
    shader->SetUniformMat4f("model", model);

    height_map->BindToSlot(0);
    std::get<0>(slope)->BindToSlot(1);
    std::get<1>(slope)->BindToSlot(2);

    renderer.DrawArrays(*vao, *shader, 0, tilingSize * tilingSize * tessendorfProperties.N * tessendorfProperties.N * 2 * 3 * 3);
}

void Ocean::OnUpdate(double deltaTime) {
    elapsedTime += deltaTime;

    textures::ssbo_pointer h_k_t = textures::generate_transform_texture(std::get<0>(spectrum_textures), std::get<1>(spectrum_textures), tessendorfProperties.N, tessendorfProperties.L, (float) elapsedTime);
    height_map = textures::update_fft_texture(h_k_t, tessendorfProperties.N);

    std::pair<textures::ssbo_pointer, textures::ssbo_pointer>
            displacement = textures::update_slope_texture(h_k_t, tessendorfProperties.N, tessendorfProperties.L);

    slope = std::make_pair(
            textures::update_fft_texture(std::get<0>(displacement), tessendorfProperties.N),
            textures::update_fft_texture(std::get<1>(displacement), tessendorfProperties.N)
            );
}

void Ocean::initializeSpectrumTextures() {
    spectrum_textures = textures::generate_spectrum_textures(
            tessendorfProperties.N,
            tessendorfProperties.A,
            tessendorfProperties.windDirection.x,
            tessendorfProperties.windDirection.y,
            tessendorfProperties.windSpeed,
            tessendorfProperties.L
            );
}

void Ocean::initializePBRShader() {
    shader = std::make_unique<abstractions::RenderingShader>(
            VERTEX_SHADER_PATH,
            FRAGMENT_SHADER_PATH);
    shader->Bind();

    // VERTEX
    shader->SetUniform1i("N", tessendorfProperties.N);
    shader->SetUniform1f("L", tessendorfProperties.L);
    shader->SetUniform1f("showBorder", showBorder);
    shader->SetUniform1f("lambda", tessendorfProperties.lambda);

    // FRAGMENT
    shader->SetUniform3f("albedo", material.albedo[0], material.albedo[1], material.albedo[2]);
    shader->SetUniform1f("metallic", material.metallic);
    shader->SetUniform1f("roughness", material.roughness);
    shader->SetUniform1f("ao", material.ao);
    shader->SetUniform3f("lightColor", material.lightColor[0], material.lightColor[1], material.lightColor[2]);
}

void Ocean::SetTessendorfProperties(TessendorfProperties _tessendorfProperties) {
    this->tessendorfProperties = _tessendorfProperties;
    initializePBRShader();
    initializeSpectrumTextures();
}

void Ocean::SetTiling(unsigned int size){
    tilingSize = size;
    initializeVertexBuffer();
}

void Ocean::SetMaterial(Material _material) {
    this->material = _material;
    initializePBRShader();
}

void Ocean::initializeVertexBuffer() {
    vao = std::make_unique<abstractions::VertexArray>();

    triangles = utils::generate_grid_mesh(tessendorfProperties.N * tilingSize, tessendorfProperties.N * tilingSize);

    std::vector<float> vertices_buffer = utils::generate_grid_buffer(triangles);

    vertexBuffer = std::make_unique<abstractions::VertexBuffer>(&vertices_buffer[0], vertices_buffer.size() * sizeof(float));

    layout = std::make_unique<VertexBufferLayout>();
    layout->Push<float>(3);
    vao->AddBuffer(*vertexBuffer, *layout);
}

void Ocean::SetShowBorder(bool show) {
    showBorder = show;
    shader->Bind();
    shader->SetUniform1f("showBorder", show);
}
