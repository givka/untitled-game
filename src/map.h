#pragma once

#include <memory>

#include <glm/vec2.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_map>

#include <SimplexNoise/src/SimplexNoise.h>

#include "sprite_renderer.h"
#include "resource_manager.h"
#include "global.h"

class Map
{
public:
    Map();

    void render();

    float frequency{ 1.0 };
    float amplitude{ 1.0 };
    float lacunarity{ 2.0 };
    float persistence{ 0.5 };
    int octaves{ 1 };
    int showed{};

    int count() { return map.size(); }

private:
    std::unordered_map<glm::vec2, float> map;
    std::unique_ptr<SpriteRenderer> renderer;
};

