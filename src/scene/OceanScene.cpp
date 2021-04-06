#include "OceanScene.h"
#include "Ocean.h"
#include "../abstractions/Renderer.h"

OceanScene::OceanScene(): camera(), children() {
    GLCall(glEnable(GL_DEPTH_TEST))
    GLCall(glEnable(GL_BLEND))
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA))

    children.push_back(std::make_unique<Ocean>(100, 100));
}

void OceanScene::OnUpdate(double deltaTime) {
    camera.OnUpdate(deltaTime);

    for(auto& node : children){
        node->OnUpdate(deltaTime);
    }
}

void OceanScene::OnRender() {
    for(auto& node : children){
        node->OnRender(camera);
    }
}

void OceanScene::OnImGuiRender() {
    for(auto& node : children){
        node->OnImGuiRender();
    }
}

void OceanScene::OnProcessInput(GLFWwindow *window) {
    camera.OnProcessInput(window);

    for(auto& node : children){
        node->OnProcessInput(window);
    }
}
