
#include <iostream>
#include <random>
#include <ctime>
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "settings.h"
#include <entt/entt.hpp>

Game::State Game::state{ ACTIVE };
std::array<bool, 1024> Game::keys{};
int Game::width{};
int Game::height{};
SpriteRenderer *Game::renderer{};
std::mt19937 Game::mersenne{
        static_cast<std::mt19937::result_type>(std::time(nullptr)) };

entt::registry Game::registry{};

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

void Game::init(int width_, int height_)
{
    width = width_;
    height = height_;

    ResourceManager::LoadShader("../data/shaders/sprite.vert",
                                "../data/shaders/sprite.frag",
                                nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width),
                                      static_cast<GLfloat>(height), 0.0f, -1.0f,
                                      1.0f);

    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    ResourceManager::LoadTexture("../data/textures/tree.png", GL_TRUE,
                                 "face");

    renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    for (auto i = 0; i < 1000; ++i)
    {
        auto entity = registry.create();
        auto pos = getRandomPos();
        auto vel = getRandomVel();
        registry.assign<position>(entity, pos.x, pos.y);
        registry.assign<velocity>(entity, vel.x, vel.y);
        registry.assign<size>(entity, 32, 32);
    }
}

void Game::destroy()
{
    delete renderer;
}

void Game::update(GLfloat dt)
{
    auto func = [dt](position &pos, velocity &vel, size &siz)
    {
        if (pos.x > 200 - siz.x && vel.dx > 0)
            vel.dx = -vel.dx;
        if (pos.x < -200 + siz.x && vel.dx < 0)
            vel.dx = -vel.dx;
        if (pos.y < -200 + siz.y && vel.dy < 0)
            vel.dy = -vel.dy;
        if (pos.y > 200 - siz.y && vel.dy > 0)
            vel.dy = -vel.dy;

        pos.x += Settings::treeSpeed * vel.dx * dt;
        pos.y += Settings::treeSpeed * vel.dy * dt;
    };

    registry.view<position, velocity, size>().each(func);
}

void Game::processInput(GLfloat dt, Camera *camera)
{
    // translation
    if (keys[GLFW_KEY_W])
        camera->pos.y -= Settings::camSpeed * dt;
    if (keys[GLFW_KEY_S])
        camera->pos.y += Settings::camSpeed * dt;
    if (keys[GLFW_KEY_D])
        camera->pos.x += Settings::camSpeed * dt;
    if (keys[GLFW_KEY_A])
        camera->pos.x -= Settings::camSpeed * dt;

    /*
    // rotation
    if (keys[GLFW_KEY_Q])
        camera->rot -= dt;
    if (keys[GLFW_KEY_E])
        camera->rot += dt;
    */
}

void Game::render(Camera *camera)
{
    glm::mat4 view = camera->getView();
    ResourceManager::GetShader("sprite").SetMatrix4("view", view);

    auto func = [](position &pos, size &siz)
    {
        renderer->DrawSprite(ResourceManager::GetTexture("face"),
                             glm::vec2(pos.x, pos.y),
                             glm::vec2(siz.x, siz.y),
                             0.0f,
                             glm::vec3(1.0f, 1.0f, 1.0f));
    };

    registry.view<position, size>().each(func);
}

glm::vec2 Game::getRandomPos()
{
    std::uniform_int_distribution bb{ -200, 200 };
    return glm::vec2(bb(mersenne), bb(mersenne));
}

glm::vec2 Game::getRandomVel()
{
    std::uniform_real_distribution angle{ 0., glm::two_pi<double>() };
    auto val = angle(mersenne);
    return glm::vec2(glm::cos(val), glm::sin(val));
}
