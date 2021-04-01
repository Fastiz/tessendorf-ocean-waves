cmake_minimum_required(VERSION 3.6)

project(opengl-test)

find_package(glfw3 3.3 REQUIRED)

find_package(GLEW REQUIRED)

include_directories(src/vendor src src/abstractions)
set(SOURCE_FILES src/Application.cpp)

file(GLOB SOURCES
        src/*.h
        src/*.cpp

        src/vendor/glm/*.h
        src/vendor/glm/*.cpp

        src/vendor/imgui/*.h
        src/vendor/imgui/*.cpp

        src/vendor/std_image/*.h
        src/vendor/std_image/*.cpp

        src/abstractions/*.h
        src/abstractions/*.cpp

        src/scenes/*.h
        src/scenes/*.cpp

        src/abstractions/primitive_shapes/*.h
        src/abstractions/primitive_shapes/*.cpp
        )

add_executable(ocean_water_simulation ${SOURCE_FILES} ${SOURCES}) # which files should produce executables

target_link_libraries(ocean_water_simulation glfw)

target_link_libraries(ocean_water_simulation GLEW)

find_package(OpenGL REQUIRED)

target_link_libraries(ocean_water_simulation OpenGL::GL)
