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

    int width, height, N;
    float L;

    double elapsedTime = 0;

    float metallic, ao, roughness;
    glm::vec3 lightColor, albedo;
public:
    Ocean(int width, int height, int N, float L);

    void OnRender(Camera& camera) override;

    void OnUpdate(double deltaTime) override;

    void OnImGuiRender();
};

#endif //OPENGL_TEST_OCEAN_H
