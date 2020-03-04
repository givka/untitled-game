#pragma  once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(int width, int height);

    glm::vec3 pos{0,0,10.0};
    glm::vec2 dim{};
    float rot{};

    glm::mat4 getView();

    glm::mat4 getProjection();

private:
    inline static constexpr float NEAR{ 0 };
    inline static constexpr float FAR{ 1000 };
};