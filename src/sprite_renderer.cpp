/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <iostream>
#include "sprite_renderer.h"
#include "map.h"

SpriteRenderer::SpriteRenderer(Shader &shader,
                               const std::vector<glm::mat4> &models,
                               const std::vector<glm::vec4> &colors,
                               const std::vector<glm::vec3> &normals)
        : shader(shader)
{
    this->initRenderData(models, colors, normals);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::initRenderData(const std::vector<glm::mat4> &models,
                                    const std::vector<glm::vec4> &colors,
                                    const std::vector<glm::vec3> &normals)
{
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = {
            // Pos      // Tex
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glBindVertexArray(this->quadVAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // INSTANCED

    glGenBuffers(1, &this->modelsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->modelsVBO);
    glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *) nullptr);
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *) (sizeof(glm::vec4)));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *) (2 * sizeof(glm::vec4)));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void *) (3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(4, 1);

    glGenBuffers(1, &this->colorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->colorsVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *) nullptr);
    glVertexAttribDivisor(5, 1);

    glGenBuffers(1, &this->normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *) nullptr);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

void SpriteRenderer::drawTiles(Texture &texture)
{
    this->shader.Use();

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, Chunk::SIZE * Chunk::SIZE);
    glBindVertexArray(0);
}

void SpriteRenderer::updateVBOs(const std::vector<glm::mat4> &models,
                                const std::vector<glm::vec4> &colors,
                                const std::vector<glm::vec3> &normals)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->modelsVBO);
    glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->colorsVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
}

