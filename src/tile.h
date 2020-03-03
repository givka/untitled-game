#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Tile
{
public :
    Tile(const glm::vec2 &position,
         const glm::vec2 &size,
         const float &angle,
         const glm::vec4 &color)
            : position(position), size(size), angle(angle), color(color)
    {

        // Prepare transformations
        this->model = glm::mat4(1.0f);
        // First translate (transformations are: scale happens first, then rotation and then final translation happens; reversed order)
        this->model = glm::translate(this->model,
                                     glm::vec3(this->position, 0.0f));
        // Move origin of rotation to center of quad
        this->model = glm::translate(this->model,
                                     glm::vec3(0.5f * this->size, 0.0f));
        // Then rotate
        this->model = glm::rotate(this->model, this->angle,
                                  glm::vec3(0.0f, 0.0f, 1.0f));
        // Move origin back
        this->model = glm::translate(this->model,
                                     glm::vec3(-0.5f * this->size, 0.0f));
        // Last scale
        this->model = glm::scale(this->model, glm::vec3(this->size, 1.0f));
    };

    static constexpr float SIZE = 32;

    glm::vec2 position;
    glm::vec2 size;
    float angle;
    glm::vec4 color;
    glm::mat4 model{};

    static glm::vec3 getColor(float noise);

};