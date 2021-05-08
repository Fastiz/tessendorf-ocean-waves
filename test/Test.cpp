#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "test_utils.h"

#include <iostream>
#include <abstractions/Renderer.h>
#include "utils/grid_test.h"
#include "abstractions/compute_shader_test.h"
#include "scene/fft/textures_test.h"

int initialize_opengl(){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Ocean Simulation Test", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    return 0;
}

int main(){
    initialize_opengl();

    grid_test::run();
    compute_shader_test::run();
    textures_test::run();

    glfwTerminate();

    return 0;
}