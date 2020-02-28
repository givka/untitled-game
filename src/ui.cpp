#include "ui.h"
#include "global.h"

Ui::Ui(GLFWwindow *window, std::string_view shaderVersion)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    io = &ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(shaderVersion.data());

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
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGui::Begin("Demo!", &showDebug, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
        ImGui::Checkbox("Demo Window", &showDemo);

        ImGui::SliderFloat("Tree speed", &Settings::treeSpeed, 0., 1000.);
        ImGui::SliderFloat("Cam speed", &Settings::camSpeed, 0., 1000.);
        ImGui::InputFloat3("Camera position", (float *) &global::camera->pos);
        ImGui::InputFloat2("Mouse position", (float *) &global::game->mousePos);

        ImGui::Separator();

        ImGui::SliderFloat("Frequency", &global::game->map->frequency, 0.001, 1.);
        ImGui::SliderFloat("Amplitude", &global::game->map->amplitude, 0.001, 10.);
        ImGui::SliderFloat("Lacunarity", &global::game->map->lacunarity, 0.001, 10.);
        ImGui::SliderFloat("Persistence", &global::game->map->persistence, 0.001, 10.);
        ImGui::SliderInt("Octaves", &global::game->map->octaves, 1, 32);

        ImGui::Separator();

        ImGui::Text("showed: %d, count: %d, ratio: %.1f",
                    global::game->map->showed,
                    global::game->map->count(),
                    100.f *
                    (static_cast<float>(global::game->map->showed ) / static_cast<float>(global::game->map->count())));

        if (ImGui::Checkbox("VSync", &Settings::isVSync))
            glfwSwapInterval(Settings::isVSync);
        ImGui::SameLine();
        ImGui::Text("%.3f ms (%.1f FPS)", 1000.0f / io->Framerate, io->Framerate);

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

