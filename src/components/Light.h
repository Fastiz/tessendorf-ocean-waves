#ifndef OPENGL_TEST_LIGHT_H
#define OPENGL_TEST_LIGHT_H


#include "../vendor/glm/vec3.hpp"

class Light {
private:
    glm::vec3 position;
    glm::vec3 color;
public:
    Light(glm::vec3 position, glm::vec3 color);

    glm::vec3 GetPosition() const;

    glm::vec3 GetColor() const;

    void SetPosition(glm::vec3 position);

    void setColor(glm::vec3 color);
};

#endif //OPENGL_TEST_LIGHT_H
