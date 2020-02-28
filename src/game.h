#pragma  once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>
#include <ctime>
#include <random>
#include <iostream>
#include <random>
#include <entt/entt.hpp>
#include "sprite_renderer.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "settings.h"
#include "camera.h"
#include "map.h"

class Map;

class Game
{
public:
    struct position
    {
        float x;
        float y;
    };

    struct velocity
    {
        float dx;
        float dy;
    };

    struct size
    {
        float x;
        float y;
    };

    struct color
    {
        float r;
        float g;
        float b;
    };

    enum State
    {
        ACTIVE,
        MENU,
        WIN
    };

    Game();

    State state = ACTIVE;
    std::array<bool, 1024> keys{};

    void destroy();

    void processInput(GLfloat dt);

    void update(GLfloat dt);

    void render();

    std::unique_ptr<Map> map{ std::make_unique<Map>() };

private :
    std::random_device randomDevice;
    std::mt19937 randgen{ randomDevice() };
    entt::registry registry{};

    glm::vec2 getRandomVel();

    static bool frustumCulled(const position &pos, const size &siz);

    float getRandomReal(float min, float max);
};
