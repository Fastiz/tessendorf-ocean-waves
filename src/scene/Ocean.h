#ifndef OPENGL_TEST_OCEAN_H
#define OPENGL_TEST_OCEAN_H

#include <memory>
#include <vector>
#include "../components/Node.h"
#include "../abstractions/Shader.h"
#include "../abstractions/VertexBuffer.h"
#include "../abstractions/VertexArray.h"
#include "../abstractions/VertexBufferLayout.h"

class Ocean : public Node {
private:
    std::unique_ptr<abstractions::Shader> shader;
    std::unique_ptr<abstractions::VertexBuffer> vertexBuffer;
    std::unique_ptr<abstractions::VertexArray> vao;
    std::unique_ptr<abstractions::VertexBufferLayout> layout;

    std::vector<std::array<glm::vec3, 3>> triangles, normals;

    int width, height;

    double elapsedTime = 0;
public:
    Ocean(int width, int height);

    void OnRender(Camera& camera) override;

    void OnUpdate(double deltaTime) override;
};

#endif //OPENGL_TEST_OCEAN_H
