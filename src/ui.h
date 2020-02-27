#pragma once

#include <string_view>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include "settings.h"

class Ui
{
public:
    Ui(GLFWwindow *window, std::string_view shaderVersion);

    void render();

    static void destroy();

private:
    bool showDebug{ true };
    bool showDemo{ false };
    ImGuiIO *io{};

    static float getPixelRatio();

    static float getHidpiScaling();
};
