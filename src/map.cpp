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

glm::vec3 getColor(float noise)
{
    if (noise < 0.)
        return glm::vec3(0, 0, 1);
    if (noise < 0.2)
        return glm::vec3(1, 1, 0);
    if (noise < 0.4)
        return glm::vec3(1, 0, 0);
    if (noise < 0.6)
        return glm::vec3(0.6, 0.1, 0.1);
    if (noise < 0.8)
        return glm::vec3(0, 0, 0);

    return glm::vec3(1, 1, 1);
}

void Map::render()
{
    int midWidth = static_cast<int>(global::camera->pos.z * 10.);
    int midHeight = midWidth;

    constexpr int SIZE{ 32 };

    this->showed = 0;

    SimplexNoise simplexNoise{ this->frequency, this->amplitude, this->lacunarity, this->persistence };

    for (int x = -midWidth + (int) (global::camera->pos.x) / SIZE;
         x <= midWidth + (int) (global::camera->pos.x) / SIZE; ++x)
    {
        for (int y = -midHeight + (int) (global::camera->pos.y) / SIZE;
             y <= midHeight + (int) (global::camera->pos.y) / SIZE; ++y)
        {
            float noise;

            // auto it = this->map.find(glm::vec2{ x, y });
            // if (it != this->map.end())
            //   noise = it->second;
            //else
            {
                noise = simplexNoise.fractal(this->octaves, (float) x, (float) y);
                this->map.insert(std::make_pair(glm::vec2(x, y), noise));
            }

            this->renderer->DrawSprite(ResourceManager::GetTexture("face"),
                                       glm::vec2(x * SIZE, y * SIZE) - glm::vec2(SIZE / 2.),
                                       glm::vec2(SIZE),
                                       0.0f,
                                       x == 0 && y == 0 ? glm::vec3(1, 0, 0) : getColor(noise));

            ++this->showed;
        }
    }
    // for (auto const&[key, val] : this->map)




}



