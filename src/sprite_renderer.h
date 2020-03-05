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
    SpriteRenderer() = default;

    SpriteRenderer(Shader &shader,
                   const std::vector<glm::mat4> &models,
                   const std::vector<glm::vec4> &colors,
                   const std::vector<glm::vec3> &normals);

    ~SpriteRenderer();

    void drawTiles(Texture &texture);

    void updateVBOs(const std::vector<glm::mat4> &models,
                    const std::vector<glm::vec4> &colors,
                    const std::vector<glm::vec3> &normals);

    Shader shader{};
private:
    GLuint quadVAO{};
    GLuint modelsVBO{};
    GLuint colorsVBO{};
    GLuint normalsVBO{};

    void initRenderData(const std::vector<glm::mat4> &models,
                        const std::vector<glm::vec4> &colors,
                        const std::vector<glm::vec3> &normals);
};
