#include "chunk.h"
#include "resource_manager.h"

float Chunk::frequency{ 0.001 };
float Chunk::amplitude{ 1.0 };
float Chunk::lacunarity{ 2.0 };
float Chunk::persistence{ 0.5 };
int Chunk::octaves{ 8 };

Chunk::Chunk(glm::vec2 pos)
        : pos(pos)
{
    for (int x = (int) this->pos.x * Chunk::SIZE; x < (int) this->pos.x * Chunk::SIZE + Chunk::SIZE; ++x)
    {
        for (int y = (int) this->pos.y * Chunk::SIZE; y < (int) this->pos.y * Chunk::SIZE + Chunk::SIZE; ++y)
        {
            auto noise = Chunk::getNoise(glm::vec2(x, y));
            float alpha = noise.z / Tile::SIZE / Tile::SIZE;

            auto position = glm::vec2(x, y);
            auto size = glm::vec2(Tile::SIZE);
            auto angle = 0.0f;
            auto color = glm::vec4(Tile::getColor(noise.z), alpha);
            auto normal = Chunk::calculateNormal(glm::vec2(x, y));

            auto tile = std::make_shared<Tile>(position, size, angle, color, normal);

            this->tiles.push_back(tile);

            this->models.push_back(tile->model);
            this->colors.push_back(tile->color);
            this->normals.push_back(tile->normal);
        }
    }
}

glm::vec3 Chunk::calculateNormal(const glm::vec2 &position)
{
    //FIXME:  if to intensive it can be simplified
    auto tl = Chunk::getNoise(position + glm::vec2(-1, 1));
    auto tc = Chunk::getNoise(position + glm::vec2(0, 1));
    auto tr = Chunk::getNoise(position + glm::vec2(1, 1));

    auto ml = Chunk::getNoise(position + glm::vec2(-1, 0));
    auto mc = Chunk::getNoise(position + glm::vec2(0, 0));
    auto mr = Chunk::getNoise(position + glm::vec2(1, 0));

    auto bl = Chunk::getNoise(position + glm::vec2(-1, -1));
    auto bc = Chunk::getNoise(position + glm::vec2(0, -1));
    auto br = Chunk::getNoise(position + glm::vec2(1, -1));

    auto normal = glm::vec3(0);
    normal += glm::cross(mr - mc, tc - mc);
    normal += glm::cross(tr - mc, tl - mc);
    normal += glm::cross(tc - mc, ml - mc);
    normal += glm::cross(tl - mc, bl - mc);
    normal += glm::cross(ml - mc, bc - mc);
    normal += glm::cross(bl - mc, br - mc);
    normal += glm::cross(bc - mc, mr - mc);
    normal += glm::cross(br - mc, tr - mc);
    return glm::normalize(normal);
}

glm::vec3 Chunk::getNoise(const glm::vec2 &position)
{
    SimplexNoise simplexNoise{ Chunk::frequency,
                               Chunk::amplitude,
                               Chunk::lacunarity,
                               Chunk::persistence };

    return glm::vec3(position, Tile::SIZE * Tile::SIZE * simplexNoise.fractal(Chunk::octaves, position.x, position.y));
}

void Chunk::createRenderer()
{
    this->renderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("sprite"),
                                                      this->models, this->colors, this->normals);

    //this->getTile(32, 32)->color.w = 1.0;
    //this->colors[32 * Chunk::SIZE + 32].w = 1.0;

    /*
    for (int y = 0; y < Chunk::SIZE; ++y)
    {
        for (int x = 0; x < Chunk::SIZE; ++x)
        {
            auto index = Chunk::getIndex(x, y);
            auto tile = this->tiles[index];

            if (tile->color.w > -0.001)
            {
                tile->color.w = -0.001f;
                tile->normal = glm::vec3(0, 0, 1);
            }

            this->models[index] = tile->model;
            this->colors[index] = tile->color;
            this->normals[index] = tile->normal;
        }
    }

    this->renderer->updateVBOs(this->models, this->colors, this->normals);
    */
}

void Chunk::render()
{
    this->renderer->drawTiles(ResourceManager::GetTexture("face"));
}

int Chunk::getIndex(int x, int y)
{
    assert(x >= 0 && x < Chunk::SIZE && y >= 0 && y < Chunk::SIZE);
    return y * Chunk::SIZE + x;
}



