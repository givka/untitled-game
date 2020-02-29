/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "sprite_renderer.h"

SpriteRenderer::SpriteRenderer(Shader &shader)
{
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::initRenderData()
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
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::drawTiles(Texture &texture)
{
    this->shader.Use();

     int MAX_SIZE = this->models.size();

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, MAX_SIZE * sizeof(glm::mat4),
                 &this->models[0], GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),
                          (void *) nullptr);
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),
                          (void *) (sizeof(glm::vec4)));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),
                          (void *) (2 * sizeof(glm::vec4)));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4),
                          (void *) (3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(4, 1);




    unsigned int bufferColor;
    glGenBuffers(1, &bufferColor);
    glBindBuffer(GL_ARRAY_BUFFER, bufferColor);
    glBufferData(GL_ARRAY_BUFFER, MAX_SIZE * sizeof(glm::vec4),
                 &this->colors[0], GL_STATIC_DRAW);

    glBindVertexArray(this->quadVAO);

    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (void *) nullptr);

    glVertexAttribDivisor(5, 1);


    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    glBindVertexArray(this->quadVAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, this->models.size());
    glBindVertexArray(0);

    this->models.clear();
    this->colors.clear();
}

void SpriteRenderer::addTile(const Tile &tile)
{
    // Prepare transformations
    glm::mat4 model = glm::mat4(1.0f);
    // First translate (transformations are: scale happens first, then rotation and then final translation happens; reversed order)
    model = glm::translate(model, glm::vec3(tile.position, 0.0f));
    // Move origin of rotation to center of quad
    model = glm::translate(model, glm::vec3(0.5f * tile.size, 0.0f));
    // Then rotate
    model = glm::rotate(model, tile.angle, glm::vec3(0.0f, 0.0f, 1.0f));
    // Move origin back
    model = glm::translate(model, glm::vec3(-0.5f * tile.size, 0.0f));
    // Last scale
    model = glm::scale(model, glm::vec3(tile.size, 1.0f));

    this->models.push_back(model);
    this->colors.push_back(tile.color);
}
