#pragma once

#include <memory>
#include "game.h"
#include "ui.h"
#include "camera.h"

namespace global
{
    inline std::unique_ptr<Game> game;
    inline std::unique_ptr<Ui> ui;
    inline std::unique_ptr<Camera> camera;
}