/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include "sprite_renderer.h"
#include <random>
#include <entt/entt.hpp>

// Represents the current state of the game


// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    enum State
    {
        ACTIVE,
        MENU,
        WIN
    };

    // Game state
    static State state;
    static std::array<bool, 1024> keys;
    static int width;
    static int height;

    static void destroy();

    static void init(int width, int height);

    static void processInput(GLfloat dt);

    static void update(GLfloat dt);

    static void render();

private :
    static SpriteRenderer *renderer;
    static std::mt19937 mersenne;
    static entt::registry registry;

    static glm::vec2 getRandomPos();
    static glm::vec2 getRandomVel();

};

#endif