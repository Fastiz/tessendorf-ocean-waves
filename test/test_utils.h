#ifndef OCEAN_WATER_SIMULATION_TEST_UTILS_H
#define OCEAN_WATER_SIMULATION_TEST_UTILS_H

#include <iostream>

#define IS_TRUE(x) { if (!(x)) std::cout << __FUNCTION__ << " failed on line " << __LINE__ << std::endl; }

#endif //OCEAN_WATER_SIMULATION_TEST_UTILS_H
