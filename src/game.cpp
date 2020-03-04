#include "game.h"

#include "globals.h"
#include <entt/entt.hpp>
#include <SimplexNoise/src/SimplexNoise.h>

Game::Game()
{

    auto midWidth = 10. * static_cast<int>(globals::camera->dim.x / 2 + 32);
    auto midHeight = 10. * static_cast<int>(globals::camera->dim.y / 2 + 32);
    for (int x = -midWidth; x < midWidth; x += 32)
    {
        for (int y = -midHeight; y < midHeight; y += 32)
        {
            auto entity = this->registry.create();
            this->registry.assign<position>(entity, x, y);
            this->registry.assign<size>(entity, 32, 32);
            this->registry.assign<color>(entity, 0, 0, 0);
        }
    }
}

void Game::destroy()
{
}

void Game::update(GLfloat)
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
        globals::camera->pos.y -= settings::camSpeed * dt;
    if (keys[GLFW_KEY_S])
        globals::camera->pos.y += settings::camSpeed * dt;
    if (keys[GLFW_KEY_D])
        globals::camera->pos.x += settings::camSpeed * dt;
    if (keys[GLFW_KEY_A])
        globals::camera->pos.x -= settings::camSpeed * dt;

    // rotation
    if (keys[GLFW_KEY_Q])
        globals::camera->rot -= dt;
    if (keys[GLFW_KEY_E])
        globals::camera->rot += dt;

    if (keys[GLFW_KEY_R])
    {
        globals::camera->rot = 0;
        globals::camera->pos = glm::vec3(0, 0, 10.0);
    }

    if (keys[GLFW_KEY_N])
    {
        ResourceManager::LoadShader("../data/shaders/sprite.vert",
                                    "../data/shaders/sprite.frag",
                                    nullptr, "sprite");
        ResourceManager::GetShader("sprite").Use()
                .SetInteger("image", 0);

        ResourceManager::GetShader("sprite")
                .SetMatrix4("projection", globals::camera->getProjection());

        for (auto&[pos, chunk] :this->map->chunks)
        {
            chunk->renderer->shader = ResourceManager::GetShader("sprite");
        }
    }

    if (this->keys[GLFW_KEY_P])
    {
        settings::sunPos.x = -1 + 2 * this->mousePos.x;
        settings::sunPos.y = -1 + 2 * this->mousePos.y;
    }
}

void Game::render()
{
    glm::mat4 view = globals::camera->getView();
    ResourceManager::GetShader("sprite").SetMatrix4("view", view);
    this->map->render();
}

glm::vec2 Game::getRandomVel()
{
    std::uniform_real_distribution angle{ 0., glm::two_pi<double>() };
    auto val = angle(this->randgen);
    return glm::vec2(glm::cos(val), glm::sin(val));
}

float Game::getRandomReal(float min, float max)
{
    std::uniform_real_distribution distribution{ min, max };
    return distribution(this->randgen);
}

bool Game::frustumCulled(const position &pos, const size &siz)
{
    return pos.x > globals::camera->pos.x + siz.x + globals::camera->pos.z * globals::camera->dim.x / 2 ||
           pos.x < globals::camera->pos.x - siz.x - globals::camera->pos.z * globals::camera->dim.x / 2 ||
           pos.y > globals::camera->pos.y + siz.y + globals::camera->pos.z * globals::camera->dim.y / 2 ||
           pos.y < globals::camera->pos.y - siz.y - globals::camera->pos.z * globals::camera->dim.y / 2;
}