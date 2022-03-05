#include <random>
#include <chrono>
#include "OceanGerstner.h"
#include "../utils/grid.h"
#include "../abstractions/VertexBufferLayout.h"
#include "../components/Camera.h"
#include "../abstractions/RenderingShader.h"

#define VERTEX_SHADER_PATH      "../../res/shaders/ocean/gerstner/pbr.vertex.shader"
#define FRAGMENT_SHADER_PATH    "../../res/shaders/ocean/gerstner/pbr.fragment.shader"

void OceanGerstner::initializePBRShader() {
    shader = std::make_unique<abstractions::RenderingShader>(
            VERTEX_SHADER_PATH,
            FRAGMENT_SHADER_PATH);
    shader->Bind();

    // VERTEX
    shader->SetUniform1i("N", N);

    // FRAGMENT
    shader->SetUniform3f("albedo", material.albedo[0], material.albedo[1], material.albedo[2]);
    shader->SetUniform1f("metallic", material.metallic);
    shader->SetUniform1f("roughness", material.roughness);
    shader->SetUniform1f("ao", material.ao);
    shader->SetUniform3f("lightColor", material.lightColor[0], material.lightColor[1], material.lightColor[2]);
}

void OceanGerstner::initializeVertexBuffer() {
    vao = std::make_unique<abstractions::VertexArray>();

    auto triangles = utils::generate_grid_mesh(N, N);

    std::vector<float> vertices_buffer = utils::generate_grid_buffer(triangles);

    vertexBuffer = std::make_unique<abstractions::VertexBuffer>(&vertices_buffer[0], vertices_buffer.size() * sizeof(float));

    layout = std::make_unique<abstractions::VertexBufferLayout>();
    layout->Push<float>(3);
    vao->AddBuffer(*vertexBuffer, *layout);
}

void OceanGerstner::OnRender(Camera &camera) {
    Renderer renderer;
    renderer.Clear();

    wavesSSBO->BindToSlot(0);

    shader->Bind();
    shader->SetUniformMat4f("view", camera.GetView());
    shader->SetUniformMat4f("proj", camera.GetProj());

    glm::vec3 cameraPos = camera.GetCameraPos();
    shader->SetUniform3f("viewPos", cameraPos[0], cameraPos[1], cameraPos[2]);

    shader->SetUniform3f("lightPosition", material.lightPosition[0], material.lightPosition[1], material.lightPosition[2]);

    shader->SetUniform1f("lightAttenuationScale", material.lightAttenuationScale);

    shader->SetUniform1f("current_time", elapsed_time);

    glm::mat4 model = glm::mat4(1.0f);
    shader->SetUniformMat4f("model", model);

    shader->SetUniform1i("number_of_waves", number_of_waves);

    wavesSSBO->BindToSlot(0);

    renderer.DrawArrays(*vao, *shader, 0, N * N * 2 * 3 * 3);
}

void OceanGerstner::OnUpdate(double deltaTime) {
    elapsed_time += time_scale * deltaTime;
    Node::OnUpdate(deltaTime);
}

void OceanGerstner::SetMaterial(Material material) {
    this->material = material;
    initializePBRShader();
}

OceanGerstner::OceanGerstner(unsigned int N, Material material, WaveConstraints waveConstraints)
    : N(N), material(material), waveConstraints(waveConstraints)
{
    initializeWaves();
    initializeVertexBuffer();
    initializePBRShader();
}

void OceanGerstner::initializeWaves() {
    std::vector<Wave> waves(number_of_waves);

    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

    std::uniform_real_distribution<float> amplitudeDistribution(waveConstraints.minAmplitude,waveConstraints.maxAmplitude);
    std::uniform_real_distribution<float> frequencyDistribution(waveConstraints.minFrequency,waveConstraints.maxFrequency);
    std::uniform_real_distribution<float> phaseDistribution(waveConstraints.minPhase,waveConstraints.maxPhase);

    static const float PI = 3.14;
    std::uniform_real_distribution<float> directionDistribution(0, 2 * PI);

    int side = (int) sqrt(number_of_waves);
    for(int i=0; i<number_of_waves; i++){
        auto & wave = waves[i];

        float directionAngle = directionDistribution(generator);

        glm::vec2 direction(std::cosf(directionAngle), std::sinf(directionAngle));
        float m = direction.y / direction.x;
        float k = wave.frequency * wave.frequency / 9.8;

        float x = k / sqrt(1 + m * m);
        wave.waveVector = glm::vec2(x, m * x);

        wave.amplitude = amplitudeDistribution(generator);
        wave.frequency = frequencyDistribution(generator);
        wave.phase = phaseDistribution(generator);
    }
    wavesSSBO = std::make_unique<SSBO>(&waves[0], sizeof(Wave) * number_of_waves, GL_STATIC_READ);
}

void OceanGerstner::SetWaveConstraints(WaveConstraints waveConstraints1) {
    this->waveConstraints = waveConstraints1;
    initializeWaves();
}

void OceanGerstner::SetTimeScale(float timeScale) {
    this->time_scale = timeScale;
}

void OceanGerstner::SetNumberOfWaves(unsigned int numberOfWaves) {
    this->number_of_waves = numberOfWaves;
    initializeWaves();
}
