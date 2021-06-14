find_package(glfw3 3.3 REQUIRED)

find_package(GLEW REQUIRED)

set(SOURCE_FILES Application.cpp)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.h" "*.cpp" "*.hpp")

add_library(Src ${SOURCES})

add_executable(ocean_water_simulation ${SOURCE_FILES} ${SOURCES}) # which files should produce executables

target_link_libraries(ocean_water_simulation imgui)

target_link_libraries(ocean_water_simulation glfw)

target_link_libraries(ocean_water_simulation GLEW)

target_link_libraries(ocean_water_simulation GLFFT)

find_package(OpenGL REQUIRED)

target_link_libraries(ocean_water_simulation OpenGL::GL)
