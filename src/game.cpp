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

    for (int x = -static_cast<int>(global::camera->dim.x / 2 + 32);
         x < static_cast<int>(global::camera->dim.x / 2 + 32);
         x += 32)
    {
        for (int y = -static_cast<int>(global::camera->dim.y / 2 + 32);
             y < static_cast<int>(global::camera->dim.y / 2 + 32);
             y += 32)
        {
            auto entity = this->registry.create();
            this->registry.assign<position>(entity, x, y);
            this->registry.assign<size>(entity, 32, 32);
            ++this->nbEntities;
        }
    }
}

void Game::destroy()
{
}

void Game::update(GLfloat dt)
{

    /*
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

     */
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

    if (keys[GLFW_KEY_R])
    {
        global::camera->rot = 0;
        global::camera->pos = glm::vec3(0, 0, 1.0);
    }
}

void Game::render()
{
    glm::mat4 view = global::camera->getView();
    ResourceManager::GetShader("sprite").SetMatrix4("view", view);

    this->nbShowed = 0;

    auto func = [this](const position &pos, const size &siz)
    {
        if (frustumCulled(pos, siz))
            return;

        ++this->nbShowed;
        this->renderer->DrawSprite(ResourceManager::GetTexture("face"),
                                   glm::vec2(pos.x, pos.y) - glm::vec2(siz.x / 2, siz.y / 2),
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

bool Game::frustumCulled(const position &pos, const size &siz)
{
    return pos.x > global::camera->pos.x + siz.x + global::camera->pos.z * global::camera->dim.x / 2 ||
           pos.x < global::camera->pos.x - siz.x - global::camera->pos.z * global::camera->dim.x / 2 ||
           pos.y > global::camera->pos.y + siz.y + global::camera->pos.z * global::camera->dim.y / 2 ||
           pos.y < global::camera->pos.y - siz.y - global::camera->pos.z * global::camera->dim.y / 2;
}