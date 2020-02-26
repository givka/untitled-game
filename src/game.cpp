
#include <iostream>
#include <random>
#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

Game::State Game::state{ ACTIVE };
std::array<bool, 1024> Game::keys{};
int Game::width{};
int Game::height{};
SpriteRenderer *Game::renderer{};
std::mt19937 Game::mersenne{ static_cast<std::mt19937::result_type>(time(nullptr)) };


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
}

void Game::destroy()
{
    delete renderer;
}

void Game::update(GLfloat dt)
{
}

void Game::processInput(GLfloat dt)
{
}

void Game::render()
{
    for (int i = 0; i < 1000; ++i)
    {
        renderer->DrawSprite(ResourceManager::GetTexture("face"),
                             getRandomPos(), glm::vec2(32, 32), 0.0f,
                             glm::vec3(1.0f, 1.0f, 1.0f));
    }
}

glm::vec2 Game::getRandomPos()
{
     std::uniform_int_distribution w{ 0, width };
     std::uniform_int_distribution h{ 0, height };
    return glm::vec2(w(mersenne),h(mersenne));
}
