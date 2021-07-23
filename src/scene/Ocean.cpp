#include <iostream>
#include "Ocean.h"
#include "../utils/grid.h"
#include "../components/Camera.h"
#include "../abstractions/RenderingShader.h"


Ocean::Ocean(int width, int height, int N, float L): width(width), height(height), N(N), L(L) {
    vao = std::make_unique<abstractions::VertexArray>();

    triangles = utils::generate_grid_mesh(width, height);

    std::vector<float> vertices_buffer = utils::generate_grid_buffer(triangles);

    vertexBuffer = std::make_unique<abstractions::VertexBuffer>(&vertices_buffer[0], vertices_buffer.size() * sizeof(float));

    layout = std::make_unique<VertexBufferLayout>();
    layout->Push<float>(3);
    vao->AddBuffer(*vertexBuffer, *layout);

    shader = std::make_unique<abstractions::RenderingShader>(
            "../../res/shaders/BasicLighting/vertex.shader",
            "../../res/shaders/BasicLighting/fragment.shader");
    shader->Bind();

    shader->SetUniform3f("objectColor", 0.31f, 0.5f, 1.0f);
    shader->SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
    shader->SetUniform1i("N", N);
    shader->SetUniform1f("L", L);

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
    shader->SetUniform3f("lightPos", lightPos[0], lightPos[1], lightPos[2]);

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
