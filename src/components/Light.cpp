#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 color)
        : position(position), color(color)
{

}

glm::vec3 Light::GetPosition() const {
    return position;
}

glm::vec3 Light::GetColor() const {
    return color;
}

void Light::SetPosition(glm::vec3 _position) {
    this->position = _position;
}

void Light::setColor(glm::vec3 _color) {
    this->color = _color;
}
