#ifndef SCENE_H
#define SCENE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Node.h"

class Scene : public Node
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void OnRender() {
    }

};

#endif //SCENE_H
