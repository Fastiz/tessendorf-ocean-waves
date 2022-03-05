#ifndef OPENGL_TEST_GRID_H
#define OPENGL_TEST_GRID_H

#include <vector>
#include <array>
#include <functional>
#include <glm/vec3.hpp>

namespace utils {
    std::vector<std::array<glm::vec3, 3>> generate_grid_mesh(unsigned int width, unsigned int height);

    std::vector<std::array<glm::vec3, 3>> generate_triangle_normals(std::vector<std::array<glm::vec3, 3>> &triangles);

    std::vector<float> generate_grid_buffer(std::vector<std::array<glm::vec3, 3>>& triangles);

    std::vector<std::array<glm::vec3, 3>> transform_grid_height(
            std::vector<std::array<glm::vec3, 3>> &triangles,
            std::function<float(float, float)> &transform
    );

    std::vector<std::array<glm::vec3, 3>>
    scale_grid(std::vector<std::array<glm::vec3, 3>> &triangles, double x, double z);
}

#endif //OPENGL_TEST_GRID_H
