#pragma once

#include <glm/vec2.hpp>
#include <glm/gtx/hash.hpp>

#include <memory>
#include <unordered_map>
#include "tile.h"
#include "sprite_renderer.h"

class Chunk
{
public:
    static constexpr int SIZE = 64;

    explicit Chunk(glm::vec2 pos);

    glm::vec2 pos;

    std::unordered_map<glm::vec2, std::shared_ptr<Tile>> tiles;

    static float frequency;
    static float amplitude;
    static float lacunarity;
    static float persistence;
    static int octaves;

    std::vector<glm::mat4> models;
    std::vector<glm::vec4> colors;

    std::unique_ptr<SpriteRenderer> renderer{};
};

