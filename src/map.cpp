#include "map.h"

Map::Map()
{

    ResourceManager::LoadShader("../data/shaders/sprite.vert",
                                "../data/shaders/sprite.frag",
                                nullptr, "sprite");
    ResourceManager::GetShader("sprite").Use()
            .SetInteger("image", 0);

    ResourceManager::GetShader("sprite")
            .SetMatrix4("projection", global::camera->getProjection());

    ResourceManager::LoadTexture("../data/textures/square.png", GL_TRUE,
                                 "face");

    this->renderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("sprite"));
}

void Map::render()
{
    int midWidth = static_cast<int>(global::camera->pos.z * 10.);
    int midHeight = midWidth;

    this->showed = 0;

    SimplexNoise simplexNoise{ this->frequency, this->amplitude, this->lacunarity, this->persistence };

    for (int x = -midWidth + (int) (global::camera->pos.x) / 32;
         x < midWidth + (int) (global::camera->pos.x) / 32; ++x)
    {
        for (int y = -midHeight + (int) (global::camera->pos.y) / 32;
             y < midHeight + (int) (global::camera->pos.y) / 32; ++y)
        {
            auto it = this->map.find(glm::vec2{ x, y });
            if (it == this->map.end())
            {
                float noise = 0.5f + 0.5f * simplexNoise.fractal(this->octaves, (float) x, (float) y);
                this->map.insert(std::make_pair(glm::vec2(x, y), noise));

                this->renderer->DrawSprite(ResourceManager::GetTexture("face"),
                                           glm::vec2(x * 32, y * 32) - glm::vec2(32 / 2.),
                                           glm::vec2(32),
                                           0.0f,
                                           glm::vec3(noise));
            } else
            {
                this->renderer->DrawSprite(ResourceManager::GetTexture("face"),
                                           glm::vec2(x * 32, y * 32) - glm::vec2(32 / 2.),
                                           glm::vec2(32),
                                           0.0f,
                                           x == 0 && y == 0 ? glm::vec3(1, 0, 0) : glm::vec3(it->second));
            }

            ++this->showed;
        }
    }
    // for (auto const&[key, val] : this->map)
}
