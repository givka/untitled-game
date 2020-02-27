//
// Created by Louis Barrière on 27/02/2020.
//

#ifndef UNTILTED_GAME_CAMERA_H
#define UNTILTED_GAME_CAMERA_H


#include <glm/glm.hpp>

class Camera
{
public:

    glm::mat4 getView();
    glm::vec2 pos{ glm::vec2(140, 140) };

    float rot{};
private:
    float width{ 800 };
    float height{ 600 };

};


#endif //UNTILTED_GAME_CAMERA_H
