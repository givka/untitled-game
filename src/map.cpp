#include <fmt/format.h>
#include "map.h"
#include "chunk.h"

int Map::NBR_CHUNKS_X = 1;
int Map::NBR_CHUNKS_Y = 1;

std::unique_ptr<Chunk> Map::test(const glm::vec2 &pos,
                                 const std::shared_ptr<Timer> &timer)
{
    timer->reset();
    auto chunk = std::make_unique<Chunk>(pos);
    timer->stop();
    return chunk;
}

Map::Map()
{
    ResourceManager::LoadShader("../data/shaders/sprite.vert",
                                "../data/shaders/sprite.frag",
                                nullptr, "sprite");
    ResourceManager::GetShader("sprite").Use()
            .SetInteger("image", 0);

    ResourceManager::GetShader("sprite")
            .SetMatrix4("projection", globals::camera->getProjection());

    ResourceManager::LoadTexture("../data/textures/square.png", GL_TRUE,
                                 "face");

//    float zoom = globals::camera->pos.z;
    this->showed = 0;

    static auto timer = globals::ui->addTimer("Map::Map()");

    timer->reset();

    this->addNeighbourChunks();

    timer->stop();
}

glm::vec3 Map::getColor(float noise)
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
    this->updateFutures();

    static auto timer = globals::ui->addTimer("Map::render()");
    timer->reset();

    this->addNeighbourChunks();

    timer->stop();
}

void Map::addNeighbourChunks()
{

    static auto camPos = globals::ui->addVec2("Map::render() camPos");
    camPos->x = (int) (globals::camera->pos.x / Chunk::SIZE / Tile::SIZE);
    camPos->y = (int) (globals::camera->pos.y / Chunk::SIZE / Tile::SIZE);

    this->showed = 0;

    for (int y = -Map::NBR_CHUNKS_Y + (int) camPos->y;
         y <= Map::NBR_CHUNKS_Y + (int) camPos->y; ++y)
    {
        for (int x = -Map::NBR_CHUNKS_X + (int) camPos->x;
             x <= Map::NBR_CHUNKS_X + (int) camPos->x; ++x)
        {
            auto itChunk = this->chunks.find(glm::vec2(x, y));
            auto itFuture = this->futures.find(glm::vec2(x, y));

            if (itChunk != this->chunks.end())
            {
                itChunk->second->renderer->drawTiles(
                        ResourceManager::GetTexture("face"));
                this->showed += itChunk->second->models.size();
                continue;
            }

            if (itFuture != this->futures.end())
                continue;

            auto p = glm::vec2(x, y);
            static auto t = globals::ui->addTimer(
                    fmt::format("Chunk: ({},{})", x, y));
            this->futures.insert(
                    std::make_pair(p, std::async(std::launch::async,
                                                 &Map::test, this, p,
                                                 t)));
        }
    }
}

void Map::updateFutures()
{
    for (auto it = this->futures.begin(); it != this->futures.end();)
    {
        auto status = it->second.wait_for(std::chrono::nanoseconds(1));
        if (status != std::future_status::ready)
            ++it;
        else
        {
            auto chunk = it->second.get();
            chunk->renderer = std::make_unique<SpriteRenderer>(
                    ResourceManager::GetShader("sprite"),
                    chunk->models,
                    chunk->colors);
            this->chunks.insert(std::make_pair(chunk->pos, std::move(chunk)));
            it = this->futures.erase(it);
        }
    }
}



