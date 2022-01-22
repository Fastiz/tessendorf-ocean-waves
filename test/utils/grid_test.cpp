#include <array>
#include <utils/grid.h>
#include "../test_utils.h"
#include "grid_test.h"

namespace grid_test {
    void triangle_count(){
        int width = 100, height = 150;
        std::vector<std::array<glm::vec3, 3>> triangles = utils::generate_grid_mesh(width, height);

        IS_TRUE(triangles.size() == width * height * 2)
    }

    void check_normals(){
        int width = 1, height = 1;
        std::vector<std::array<glm::vec3, 3>> triangles = utils::generate_grid_mesh(width, height);
        std::vector<std::array<glm::vec3, 3>> normals = utils::generate_triangle_normals(triangles);

        const double EPSILON = 0.00000001;

        for(auto& triangle_normals : normals){
            for(auto& normal : triangle_normals){
                IS_TRUE(std::abs(normal[0]) < EPSILON)
                IS_TRUE(std::abs(normal[1] - 1.0) < EPSILON)
                IS_TRUE(std::abs(normal[2]) < EPSILON)
            }
        }
    }

    void check_buffer_length(){
        int width = 1, height = 1;
        std::vector<std::array<glm::vec3, 3>> triangles = utils::generate_grid_mesh(width, height);
        std::vector<std::array<glm::vec3, 3>> normals = utils::generate_triangle_normals(triangles);

        std::vector<float> buffer = utils::generate_grid_buffer(triangles);

        unsigned int buffer_size = buffer.size();

        IS_TRUE(buffer.size() == width * height * 2 * 3 * 3)
    }

    void run(){
        triangle_count();
        check_normals();
        check_buffer_length();
    }
}