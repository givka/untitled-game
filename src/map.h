#pragma once

#include <memory>

#include <glm/vec2.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_map>

#include <SimplexNoise/src/SimplexNoise.h>

#include "sprite_renderer.h"
#include "resource_manager.h"
#include "globals.h"
#include "chunk.h"
#include "timer.h"
#include <future>

class Map
{
public:
    Map();

    void render();

    int count() { return this->chunks.size(); }

    static  int NBR_CHUNKS_X;
    static  int NBR_CHUNKS_Y;

    int showed{};

    std::unordered_map<glm::vec2, std::unique_ptr<Chunk>> chunks;
private:

    glm::vec3 getColor(float noise);

    std::unique_ptr<Chunk> test(const glm::vec2 &pos,
                                const std::shared_ptr<Timer> &timer);

    std::unordered_map<glm::vec2, std::future<std::unique_ptr<Chunk>>> futures{};

    void addNeighbourChunks();

    bool lol = false;

    void updateFutures();
};

