#pragma once

#include <string_view>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <vector>
#include <memory>
#include <glm/vec2.hpp>
#include "settings.h"
#include "timer.h"
#include "globals.h"
#include <glm/gtx/string_cast.hpp>


class Ui
{
public:
    Ui(GLFWwindow *window, std::string_view shaderVersion);

    void render();

    static void destroy();

    std::shared_ptr<Timer> addTimer(const std::string &name);
    std::shared_ptr<glm::vec2> addVec2(const std::string &name);

    std::vector<std::pair<std::shared_ptr<Timer>, std::string>> timers;
    std::vector<std::pair<std::shared_ptr<glm::vec2>, std::string>> vec2s;
private:
    bool showDebug{ true };
    bool showDemo{ false };

    ImGuiIO *io{};

    static float getPixelRatio();

    static float getHidpiScaling();
};
