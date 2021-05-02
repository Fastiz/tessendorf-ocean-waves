#include "Computer.h"
#include "ComputeShader.h"
#include "Renderer.h"

namespace abstractions {
    void
    Computer::DispatchCompute(ComputeShader &shader, int num_groups_x, int num_groups_y, int num_groups_z) {
        shader.Bind();

        GLCall(glDispatchCompute(num_groups_x, num_groups_y, num_groups_z));
    }

}
