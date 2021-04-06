find_package(glfw3 3.3 REQUIRED)

find_package(GLEW REQUIRED)

set(SOURCE_FILES Application.cpp)

file(GLOB SOURCES
        *.h
        *.cpp

        vendor/glm/*.h
        vendor/glm/*.cpp

        vendor/imgui/*.h
        vendor/imgui/*.cpp

        vendor/std_image/*.h
        vendor/std_image/*.cpp

        abstractions/*.h
        abstractions/*.cpp

        scene/*.h
        scene/*.cpp

        abstractions/primitive_shapes/*.h
        abstractions/primitive_shapes/*.cpp

        utils/*.h
        utils/*.cpp

        components/*.h
        components/*.cpp
        )

add_library(Src ${SOURCES})

add_executable(ocean_water_simulation ${SOURCE_FILES} ${SOURCES}) # which files should produce executables

target_link_libraries(ocean_water_simulation glfw)

target_link_libraries(ocean_water_simulation GLEW)

find_package(OpenGL REQUIRED)

target_link_libraries(ocean_water_simulation OpenGL::GL)
