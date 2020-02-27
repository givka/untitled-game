//
// Created by Louis Barrière on 27/02/2020.
//

#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "game.h"

glm::mat4 Camera::getView()
{
    glm::mat4 view = glm::mat4(1);
    glm::vec2 midOffset = glm::vec2(Game::width * 0.5, Game::height * 0.5);

    view = glm::translate(view, glm::vec3(midOffset, 0.0));
    view = glm::rotate(view, -this->rot, glm::vec3(0.0, 0.0, 1.0));
    view = glm::translate(view, glm::vec3(-midOffset, 0.0));
    view = glm::translate(view, glm::vec3(-this->pos, 1.0));

    return view;
}
