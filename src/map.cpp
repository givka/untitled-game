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

    this->renderer = std::make_unique<SpriteRenderer>(
            ResourceManager::GetShader("sprite"));
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
    constexpr int SIZE{ 32 };

    float zoom = global::camera->pos.z;
    auto view = 1.f + zoom * global::camera->dim / (2.f * SIZE);
    this->showed = 0;

    SimplexNoise simplexNoise{ this->frequency, this->amplitude,
                               this->lacunarity, this->persistence };

    for (int x = (int) (-view.x + global::camera->pos.x / SIZE);
         x <= (int) (view.x + global::camera->pos.x / SIZE); ++x)
    {
        for (int y = (int) (-view.y + (global::camera->pos.y) / SIZE);
             y <= (int) (view.y + (global::camera->pos.y) / SIZE); ++y)
        {
            float noise;

            // auto it = this->map.find(glm::vec2{ x, y });
            // if (it != this->map.end())
            //   noise = it->second;
            //else
            {
                noise = simplexNoise.fractal(this->octaves, (float) x,
                                             (float) y);
                this->map.insert(std::make_pair(glm::vec2(x, y), noise));
            }

            auto position =
                    (float) SIZE * glm::vec2(x, y) - glm::vec2(SIZE / 2.);

            auto size = glm::vec2(SIZE);
            auto angle = 0.0f;
            auto color = x == 0 && y == 0 ? glm::vec3(1, 0, 0) : getColor(
                    noise);
            auto tile = Tile{ position, size, angle, glm::vec4(color, .5+.5*noise) };

            this->renderer->addTile(tile);

            ++this->showed;
        }
    }

    this->renderer->drawTiles(ResourceManager::GetTexture("face"));
    // for (auto const&[key, val] : this->map)
}



