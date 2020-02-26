#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <string_view>
#include <GLFW/glfw3.h>
#include "ui.h"
#include "settings.h"

bool Ui::showDebug = true;
bool Ui::showDemo = false;
ImGuiIO *Ui::io{ nullptr };

void Ui::init(GLFWwindow *window, std::string_view glslVersion)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    io = &ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion.data());

    float hidpiScaling = getHidpiScaling();
    float pixelRatio = getPixelRatio();
    io->Fonts->Clear();
    io->Fonts->AddFontFromFileTTF("../includes/imgui/misc/fonts/DroidSans.ttf",
                                  14 * hidpiScaling);
    io->FontGlobalScale = 1.0f / pixelRatio;
}

void Ui::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (showDebug)
    {
        ImGui::Begin("Demo!", &showDebug);
        ImGui::Checkbox("Demo Window", &showDemo);
        if (ImGui::Checkbox("VSync", &Settings::isVSync)){
            glfwSwapInterval(Settings::isVSync);
        }


        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (showDemo)
        ImGui::ShowDemoWindow(&showDemo);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Ui::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

float Ui::getPixelRatio()
{
    int buf_size[2];
    int win_size[2];
    GLFWwindow *window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &buf_size[0], &buf_size[1]);
    glfwGetWindowSize(window, &win_size[0], &win_size[1]);
    return (float) buf_size[0] / (float) win_size[0];
}

float Ui::getHidpiScaling()
{
    float xScale, yScale;
    GLFWwindow *window = glfwGetCurrentContext();
    glfwGetWindowContentScale(window, &xScale, &yScale);
    return 0.5f * (xScale + yScale);
}
