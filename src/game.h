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

    int nbShowed{};
    int nbEntities{  };

private :
    std::unique_ptr<SpriteRenderer> renderer;
    std::random_device randomDevice;
    std::mt19937 randgen{ randomDevice() };
    entt::registry registry{};

    glm::vec2 getRandomVel();

    static bool frustumCulled(const position &pos, const size &siz);
};
