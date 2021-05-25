cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 20)

project(ocean_water_simulation)

# GLM LIBRARY
file(GLOB_RECURSE GLM_SOURCE_FILES CONFIGURE_DEPENDS "submodules/glm/glm/*.h" "submodules/glm/glm/*.cpp" "submodules/glm/glm/*.hpp")
add_library(glm ${GLM_SOURCE_FILES})
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

add_subdirectory(src)
add_subdirectory(test)