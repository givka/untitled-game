#include <iostream>
#include "chunk.h"
#include "settings.h"

float Chunk::frequency{ 0.001 };
float Chunk::amplitude{ 1.0 };
float Chunk::lacunarity{ 2.0 };
float Chunk::persistence{ 0.5 };
int Chunk::octaves{ 8 };

Chunk::Chunk(glm::vec2 pos)
        : pos(pos)
{

    for (int x = -Chunk::SIZE / 2 + (int) this->pos.x * Chunk::SIZE;
         x < Chunk::SIZE / 2 + (int) this->pos.x * Chunk::SIZE; ++x)
    {
        for (int y = -Chunk::SIZE / 2 + (int) this->pos.y * Chunk::SIZE;
             y < Chunk::SIZE / 2 + (int) this->pos.y * Chunk::SIZE; ++y)
        {
            auto noise = Chunk::getNoise(glm::vec2(x, y));
            float alpha = noise.z / Tile::SIZE / Tile::SIZE;

            auto position = Tile::SIZE * glm::vec2(x, y) - Tile::SIZE / 2;
            auto size = glm::vec2(Tile::SIZE);
            auto angle = 0.0f;
            auto color = glm::vec4(Tile::getColor(noise.z), alpha);
            auto normal = Chunk::calculateNormal(glm::vec2(x, y));

            auto tile = std::make_shared<Tile>(position, size, angle, color, normal);

            this->tiles.insert(std::make_pair(glm::vec2(x, y), tile));

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


