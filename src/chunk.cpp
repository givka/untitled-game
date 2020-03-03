
#include <SimplexNoise/src/SimplexNoise.h>
#include "chunk.h"
#include "global.h"

float Chunk::frequency{ 0.001 };
float Chunk::amplitude{ 1.0 };
float Chunk::lacunarity{ 2.0 };
float Chunk::persistence{ 0.5 };
int Chunk::octaves{ 8 };

Chunk::Chunk(glm::vec2 pos)
        : pos(pos)
{
    SimplexNoise simplexNoise{ Chunk::frequency,
                               Chunk::amplitude,
                               Chunk::lacunarity,
                               Chunk::persistence };

    for (int x = -Chunk::SIZE / 2 + (int) this->pos.x * Chunk::SIZE;
         x < Chunk::SIZE / 2 + (int) this->pos.x * Chunk::SIZE;
         ++x)
    {
        for (int y = -Chunk::SIZE / 2 + (int) this->pos.y * Chunk::SIZE;
             y < Chunk::SIZE / 2 + (int) this->pos.y * Chunk::SIZE;
             ++y)
        {
            float noise = simplexNoise.fractal(Chunk::octaves,
                                               (float) x, (float) y);
            float alpha = .5f + .5f * noise;
            auto position = Tile::SIZE * glm::vec2(x, y) - Tile::SIZE / 2;
            auto size = glm::vec2(Tile::SIZE);
            auto angle = 0.0f;
            auto color = glm::vec4(Tile::getColor(noise), alpha);

            auto tile = std::make_shared<Tile>(position,
                                               size,
                                               angle,
                                               color);

            this->tiles.insert(std::make_pair(glm::vec2(x, y), tile));

            this->models.push_back(tile->model);
            this->colors.push_back(tile->color);
        }
    }
}
