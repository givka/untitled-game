#ifndef UNTILTED_GAME_UI_H
#define UNTILTED_GAME_UI_H

class Ui
{
public:
    static void init(GLFWwindow *window, std::string_view shaderVersion);

    static void render();

    static void destroy();

private:
    static bool showDebug;
    static bool showDemo;
    static ImGuiIO *io;

    static float getPixelRatio();

    static float getHidpiScaling();
};

#endif //UNTILTED_GAME_UI_H
