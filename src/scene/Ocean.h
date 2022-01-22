#ifndef OPENGL_TEST_OCEAN_H
#define OPENGL_TEST_OCEAN_H

#include <memory>
#include <vector>
#include "../components/Node.h"
#include "../abstractions/Shader.h"
#include "../abstractions/VertexBuffer.h"
#include "../abstractions/VertexArray.h"
#include "../abstractions/VertexBufferLayout.h"
#include "../abstractions/SSBO.h"
#include "./fft/textures.h"

struct Material {
    float metallic, ao, roughness;
    glm::vec3 lightColor, albedo;
    glm::vec3 lightPosition;
    float lightAttenuationScale;
};
struct TessendorfProperties {
    int N;
    float L;
    float A;
    glm::vec2 windDirection;
    float windSpeed;
};

class Ocean : public Node {
private:
    std::unique_ptr<abstractions::Shader> shader;
    std::unique_ptr<abstractions::VertexBuffer> vertexBuffer;
    std::unique_ptr<abstractions::VertexArray> vao;
    std::unique_ptr<abstractions::VertexBufferLayout> layout;

    std::vector<std::array<glm::vec3, 3>> triangles, normals;

    std::pair<textures::ssbo_pointer, textures::ssbo_pointer> spectrum_textures;
    textures::ssbo_pointer height_map;
    std::pair<textures::ssbo_pointer, textures::ssbo_pointer> slope;

    double elapsedTime = 0;

    struct TessendorfProperties tessendorfProperties;
    struct Material material;
    unsigned int tilingSize;
    bool showBorder;
    float choppyWavesLambda;
    float timeScale;
    float oceanScale;
public:
    Ocean(TessendorfProperties tessendorfProperties, Material material, unsigned int tilingSize, bool isShowBorder, float choppyWavesLambda, float timeScale, float oceanScale);

    void OnRender(Camera& camera) override;

    void OnUpdate(double deltaTime) override;

    void SetTessendorfProperties(TessendorfProperties tessendorfProperties);
    void SetMaterial(Material material);
    void SetTiling(unsigned int tilingSize);
    void SetShowBorder(bool show);
    void SetChoppyWavesLambda(float lambda);
    void SetTimeScale(float timeScale_);
    void SetOceanScale(float oceanScale);
private:
    void initializeSpectrumTextures();
    void initializePBRShader();
    void initializeVertexBuffer();
};

#endif //OPENGL_TEST_OCEAN_H
