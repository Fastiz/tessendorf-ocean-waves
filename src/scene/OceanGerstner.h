#ifndef OCEAN_WATER_SIMULATION_OCEANGERSTNER_H
#define OCEAN_WATER_SIMULATION_OCEANGERSTNER_H

#include <memory>
#include "../components/Node.h"
#include "../abstractions/Shader.h"
#include "../abstractions/VertexBuffer.h"
#include "../abstractions/VertexArray.h"
#include "OceanFFT.h"

struct Wave {
    glm::vec2 waveVector;
    float amplitude;
    float frequency;
    float phase;
};

struct WaveConstraints {
    float minAmplitude, maxAmplitude;
    float minFrequency, maxFrequency;
    float minPhase, maxPhase;
};

class OceanGerstner : public Node {
private:
    std::unique_ptr<abstractions::Shader> shader;
    std::unique_ptr<abstractions::VertexBuffer> vertexBuffer;
    std::unique_ptr<abstractions::VertexArray> vao;
    std::unique_ptr<abstractions::VertexBufferLayout> layout;
    std::unique_ptr<abstractions::SSBO> wavesSSBO;

    unsigned int N;

    Material material;

    unsigned int number_of_waves = 100;
    WaveConstraints waveConstraints;

    float elapsed_time = 0;
    float time_scale = 1.0f;
public:
    OceanGerstner(unsigned int N, Material material, WaveConstraints waveConstraints);

    void OnRender(Camera& camera) override;
    void OnUpdate(double deltaTime) override;

    void SetMaterial(Material material);

    void SetWaveConstraints(WaveConstraints waveConstraints);
    void SetTimeScale(float time_scale);
    void SetNumberOfWaves(unsigned int numberOfWaves);
private:
    void initializePBRShader();
    void initializeVertexBuffer();
    void initializeWaves();
};


#endif //OCEAN_WATER_SIMULATION_OCEANGERSTNER_H
