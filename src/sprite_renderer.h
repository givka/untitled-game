#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"
#include "tile.h"

class SpriteRenderer
{
public:
    // Constructor (inits shaders/shapes)
    SpriteRenderer() = default;

    SpriteRenderer(Shader &shader);

    // Destructor
    ~SpriteRenderer();

    void drawTiles(Texture &texture);

    void addTile(const Tile& tile);

private:

    std::vector<glm::mat4> models;
    std::vector<glm::vec4> colors;

    // Render state
    Shader shader;
    GLuint quadVAO;

    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData();

};
