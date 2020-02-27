#include "game.h"

#include "global.h"
#include <entt/entt.hpp>

Game::Game()
{
    ResourceManager::LoadShader("../data/shaders/sprite.vert",
                                "../data/shaders/sprite.frag",
                                nullptr, "sprite");
    ResourceManager::GetShader("sprite")
            .Use()
            .SetInteger("image", 0);
    ResourceManager::GetShader("sprite")
            .SetMatrix4("projection", global::camera->getProjection());

    ResourceManager::LoadTexture("../data/textures/square.png", GL_TRUE,
                                 "face");

    this->renderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("sprite"));

    for (auto i = 0; i < this->nbEntities; ++i)
    {
        auto entity = this->registry.create();
        auto vel = getRandomVel();
        this->registry.assign<position>(entity, 0, 0);
        this->registry.assign<velocity>(entity, vel.x, vel.y);
        this->registry.assign<size>(entity, 32, 32);
    }
}

void Game::destroy()
{
}

void Game::update(GLfloat dt)
{
    auto func = [dt](auto &pos, auto &vel, auto &siz)
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

    this->registry.view<position, velocity, size>().each(func);
}

void Game::processInput(GLfloat dt)
{
    // translation
    if (keys[GLFW_KEY_W])
        global::camera->pos.y -= Settings::camSpeed * dt;
    if (keys[GLFW_KEY_S])
        global::camera->pos.y += Settings::camSpeed * dt;
    if (keys[GLFW_KEY_D])
        global::camera->pos.x += Settings::camSpeed * dt;
    if (keys[GLFW_KEY_A])
        global::camera->pos.x -= Settings::camSpeed * dt;

    // rotation
    if (keys[GLFW_KEY_Q])
        global::camera->rot -= dt;
    if (keys[GLFW_KEY_E])
        global::camera->rot += dt;
}

void Game::render()
{
    glm::mat4 view = global::camera->getView();
    ResourceManager::GetShader("sprite").SetMatrix4("view", view);

    this->nbShowed = 0;

    auto func = [this](const position &pos, const size &siz)
    {
        if (frustrumCulled(pos, siz))
            return;

        ++this->nbShowed;
        this->renderer->DrawSprite(ResourceManager::GetTexture("face"),
                                   glm::vec2(pos.x, pos.y),
                                   glm::vec2(siz.x, siz.y),
                                   0.0f,
                                   glm::vec3(1.0f, 1.0f, 1.0f));
    };

    this->registry.view<position, size>().each(func);
}

glm::vec2 Game::getRandomVel()
{
    std::uniform_real_distribution angle{ 0., glm::two_pi<double>() };
    auto val = angle(this->randgen);
    return glm::vec2(glm::cos(val), glm::sin(val));
}

bool Game::frustrumCulled(const position &pos, const size &siz)
{
    return pos.x > global::camera->pos.x - siz.x / 2 + global::camera->dim.x / 2 ||
           pos.x < global::camera->pos.x - siz.x / 2 - global::camera->dim.x / 2 ||
           pos.y > global::camera->pos.y - siz.y / 2 + global::camera->dim.y / 2 ||
           pos.y < global::camera->pos.y - siz.y / 2 - global::camera->dim.y / 2;
}