#ifndef OCEAN_WATER_SIMULATION_COMPUTER_H
#define OCEAN_WATER_SIMULATION_COMPUTER_H

namespace abstractions {
    class ComputeShader;

    class Computer {
    public:
        void DispatchCompute(ComputeShader& shader, int num_groups_x, int num_groups_y, int num_groups_z);

        static void MemoryBarrier();
    };

}

#endif //OCEAN_WATER_SIMULATION_COMPUTER_H
