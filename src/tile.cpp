//
// Created by Louis Barrière on 28/02/2020.
//

#include "tile.h"

glm::vec3 Tile::getColor(float noise)
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
