#pragma once

#include <memory>
#include "game.h"
#include "ui.h"
#include "camera.h"
#include "map.h"

class Game;
class Ui;
namespace globals
{
    inline std::unique_ptr<Game> game;
    inline std::unique_ptr<Ui> ui;
    inline std::unique_ptr<Camera> camera;
}
