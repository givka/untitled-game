#pragma  once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(int width, int height);

    glm::vec2 pos{ };
    glm::vec2 dim{ };
    float rot{};

    glm::mat4 getView();

    glm::mat4 getProjection();
};