#pragma once

#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "texture.h"
#include "shader.h"
#include "tile.h"

class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer() = default;

    SpriteRenderer(Shader &shader, std::vector<glm::mat4> &models,
                   std::vector<glm::vec4> &colors, std::vector<glm::vec3> &normals);

    ~SpriteRenderer();

    void drawTiles(Texture &texture);

    Shader shader{};
private:
    std::vector<glm::mat4> models{};
    std::vector<glm::vec4> colors{};
    std::vector<glm::vec3> normals{};

    GLuint quadVAO{};

    void initRenderData();
};
