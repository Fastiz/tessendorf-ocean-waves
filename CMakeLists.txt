cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 20)

project(ocean_water_simulation)

include_directories(submodules/includes)

# GLFW
add_subdirectory(submodules/glfw)
include_directories(submodules/glfw/include)

# GLEW
add_subdirectory(submodules/GLEW)
include_directories(submodules/GLEW/include)

# GLM LIBRARY
add_subdirectory(submodules/glm)
include_directories(submodules/glm)

# STB LIBRARY
file(GLOB STB_SOURCE_FILES submodules/stb/*.h submodules/stb/stb_vorbis.c)
add_library(stb ${STB_SOURCE_FILES})
set_target_properties(stb PROPERTIES LINKER_LANGUAGE CXX)
include_directories(submodules/stb)

# IMGUI LIBRARY
file(GLOB IMGUI_SOURCE_FILES
        submodules/imgui/*.h
        submodules/imgui/*.cpp
        submodules/imgui/*.hpp

        submodules/imgui/backends/imgui_impl_glfw.cpp
        submodules/imgui/backends/imgui_impl_glfw.h

        submodules/imgui/backends/imgui_impl_opengl3.cpp
        submodules/imgui/backends/imgui_impl_opengl3.h
        )
add_library(imgui ${IMGUI_SOURCE_FILES})
include_directories(submodules/imgui)

set(CMAKE_CXX_STANDARD 11)

# GLFFT LIBRARY
file(GLOB GLFFT_SOURCE_FILES submodules/GLFFT/*.cpp)
add_library(GLFFT ${GLFFT_SOURCE_FILES})
include_directories(submodules/GLFFT)
add_custom_command(
        OUTPUT submodules/GLFFT/glsl
        WORKING_DIR submodules/GLFFT/glsl
        COMMAND make
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(test)