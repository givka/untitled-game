//
// Created by Louis Barrière on 27/02/2020.
//

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

Camera::Camera(int width, int height)
        : dim(glm::vec2(width, height))
{
}

glm::mat4 Camera::getProjection()
{
    return glm::ortho(0.0f,
                      static_cast<float>(this->dim.x),
                      static_cast<float>(this->dim.y),
                      0.0f, NEAR, FAR);
}

glm::mat4 Camera::getView()
{
    glm::mat4 view = glm::mat4(1);
    glm::vec2 midOffset = glm::vec2(this->dim.x * 0.5, this->dim.y * 0.5);

    view = glm::translate(view, glm::vec3(midOffset, 0.0));
    view = glm::scale(view, glm::vec3(1.0/this->pos.z));
    view = glm::rotate(view, -this->rot, glm::vec3(0.0, 0.0, 1.0));
    view = glm::translate(view, glm::vec3(-midOffset, 0.0));
    view = glm::translate(view, -glm::vec3(glm::vec2(this->pos.x, this->pos.y) - midOffset, 1.0));

    return view;
}

