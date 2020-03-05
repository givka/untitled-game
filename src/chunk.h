#pragma once

#include <glm/vec2.hpp>
#include <glm/gtx/hash.hpp>
#include <SimplexNoise/src/SimplexNoise.h>

#include <memory>
#include <unordered_map>
#include "tile.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

class Chunk
{
public:
    static constexpr int SIZE = 64;

    explicit Chunk(glm::vec2 pos);

    glm::vec2 pos;

    static float frequency;
    static float amplitude;
    static float lacunarity;
    static float persistence;
    static int octaves;

    void createRenderer();

    int numberOfTiles() { return this->tiles.size(); }

    void render();

    void reloadShader() { this->renderer->shader = ResourceManager::GetShader("sprite"); }

    static int getIndex(int x, int y);

private:
    std::unique_ptr<SpriteRenderer> renderer{};

    std::vector<std::shared_ptr<Tile>> tiles;
    std::vector<glm::mat4> models;
    std::vector<glm::vec4> colors;
    std::vector<glm::vec3> normals;

    static glm::vec3 getNoise(const glm::vec2 &pos);

    static glm::vec3 calculateNormal(const glm::vec2 &position);
};

