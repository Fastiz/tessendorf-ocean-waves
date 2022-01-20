#include <functional>
#include "grid.h"
#include <glm/ext.hpp>

namespace utils {
    /*
     * https://gamedev.stackexchange.com/questions/133864/calculating-vertex-normals-in-opengl-c
     * */
    static glm::vec3 calculate_normal(glm::vec3 i, glm::vec3 i1, glm::vec3 i2){
        glm::vec3   v1 = i1 - i,
                    v2 = i2 - i;

        glm::vec3 v3 = glm::cross(v1, v2);

        return glm::normalize(v3);
    }

    std::vector<std::array<glm::vec3, 3>> generate_grid_mesh(
            int width,
            int height
    ){
        std::vector<std::array<glm::vec3, 3>> triangles;
        triangles.reserve((height+1) * (width+1) * 2);
        for(int z = 0; z <= height - 1; z++) {
            for (int x = 0; x <= width - 1; x++) {
                std::array<glm::vec3, 3> right{};
                std::array<glm::vec3, 3> left{};

                left[0] = {x, 0, z};
                left[1] = {x, 0, z + 1};
                left[2] = {x + 1, 0, z + 1};

                right[0] = {x, 0, z};
                right[1] = {x + 1, 0, z + 1};
                right[2] = {x + 1, 0, z};

                triangles.push_back(left);
                triangles.push_back(right);
            }
        }

        return triangles;
    }

    std::vector<std::array<glm::vec3, 3>> generate_triangle_normals(std::vector<std::array<glm::vec3, 3>>& triangles){
        std::vector<std::array<glm::vec3, 3>> normals;

        for(auto& triangle : triangles){
            std::array<glm::vec3, 3> new_normals{};

            new_normals[0] = calculate_normal(triangle[0], triangle[1], triangle[2]);
            new_normals[1] = calculate_normal(triangle[1], triangle[2], triangle[0]);
            new_normals[2] = calculate_normal(triangle[2], triangle[0], triangle[1]);

            normals.push_back(new_normals);
        }

        return normals;
    }

    std::vector<float> generate_grid_buffer(std::vector<std::array<glm::vec3, 3>>& triangles){
        std::vector<float> buffer;

        for(auto & triangle : triangles){
            for(int j=0; j<3; j++){
                for(int k=0; k<3; k++){
                    buffer.push_back(triangle[j][k]);
                }
            }
        }

        return buffer;
    }

    std::vector<std::array<glm::vec3, 3>> transform_grid_height(
            std::vector<std::array<glm::vec3, 3>>& triangles,
            std::function<float(float, float)>& transform
            ){
        std::vector<std::array<glm::vec3, 3>> result;
        for(auto& triangle: triangles){
            std::array<glm::vec3, 3> new_triangle{};
            for(int i=0; i<3; i++){
                glm::vec3& vertex = triangle[i];

                new_triangle[i] = {vertex[0], transform(vertex[0], vertex[2]), vertex[2]};
            }
            result.push_back(new_triangle);
        }
        return result;
    }

    std::vector<std::array<glm::vec3, 3>> scale_grid(std::vector<std::array<glm::vec3, 3>>& triangles, double x, double z){
        std::vector<std::array<glm::vec3, 3>> result;
        for(auto& triangle: triangles){
            std::array<glm::vec3, 3> new_triangle{};
            for(int i=0; i<3; i++){
                glm::vec3& vertex = triangle[i];

                new_triangle[i] = {vertex[0]*x, vertex[1], vertex[2]*z};
            }
            result.push_back(new_triangle);
        }
        return result;
    }
}