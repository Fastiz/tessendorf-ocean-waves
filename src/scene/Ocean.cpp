#include <iostream>
#include "Ocean.h"
#include "../utils/grid.h"
#include "../components/Camera.h"
#include "../abstractions/VertexArray.h"
#include "../abstractions/Shader.h"
#include "../abstractions/VertexBufferLayout.h"

Ocean::Ocean(int width, int height): width(width), height(height) {
    vao = std::make_unique<abstractions::VertexArray>();

    triangles = utils::generate_grid_mesh(width, height);
    triangles = utils::scale_grid(triangles, 100.0f, 100.0f);

    normals = utils::generate_triangle_normals(triangles);

    std::vector<float> vertices_buffer = utils::generate_grid_buffer(triangles, normals);

    vertexBuffer = std::make_unique<abstractions::VertexBuffer>(&vertices_buffer[0], vertices_buffer.size() * sizeof(float));

    layout = std::make_unique<VertexBufferLayout>();
    layout->Push<float>(3);
    layout->Push<float>(3);
    vao->AddBuffer(*vertexBuffer, *layout);

    shader = std::make_unique<abstractions::Shader>("../../res/shaders/BasicLighting.shader");
    shader->Bind();

    shader->SetUniform3f("objectColor", 0.31f, 0.5f, 1.0f);
    shader->SetUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
}

void Ocean::OnRender(Camera& camera) {
    Renderer renderer;
    renderer.Clear();

    shader->Bind();
    shader->SetUniformMat4f("view", camera.GetView());
    shader->SetUniformMat4f("proj", camera.GetProj());

    glm::vec3 cameraPos = camera.GetCameraPos();
    shader->SetUniform3f("viewPos", cameraPos[0], cameraPos[1], cameraPos[2]);

    glm::vec3 lightPos = {0.0f, 40.0f, 0.0f};
    shader->SetUniform3f("lightPos", lightPos[0], lightPos[1], lightPos[2]);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -10.0f, 0.0f));
    shader->SetUniformMat4f("model", model);

    renderer.DrawArrays(*vao, *shader, 0, width * height * 2 * 3);
}

float sinusoid(float x, float z, float a, float b, float delta){
    return std::sin(x + z + delta);
}

void Ocean::OnUpdate(double deltaTime) {
    elapsedTime += deltaTime;

    float a = 100, b = 100;
    std::function<float(float, float)> lambda = [&](float x, float z){
        return sinusoid(x, z, a, b, (float) elapsedTime);
    };

    triangles = utils::transform_grid_height(triangles, lambda);
    normals = utils::generate_triangle_normals(triangles);

    std::vector<float> vertices_buffer = utils::generate_grid_buffer(triangles, normals);

    vertexBuffer = std::make_unique<abstractions::VertexBuffer>(&vertices_buffer[0], vertices_buffer.size() * sizeof(float));

    vao->AddBuffer(*vertexBuffer, *layout);
}
