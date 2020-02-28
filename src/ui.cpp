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

        ImGui::Text("showed: %d (%.1f )",
                    global::game->nbShowed,
                    100.f *
                    (static_cast<float>(global::game->nbShowed ) / static_cast<float>(global::game->nbEntities)));

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

