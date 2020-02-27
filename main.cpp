#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "src/ui.h"
#include "src/resource_manager.h"
#include "src/settings.h"
#include "src/camera.h"
#include "src/global.h"

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;

void errorCallback(int error, const char *description)
{
    fprintf(stderr, "glfw Error %d: %s\n", error, description);
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    global::camera->dim = glm::vec2(width, height);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", global::camera->getProjection());

    glViewport(0, 0, width, height);
    glfwSwapBuffers(window);
}

void keyCallback(GLFWwindow *window, int key, int, int action, int)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            global::game->keys[key] = GL_TRUE;
        else if (action == GLFW_RELEASE)
            global::game->keys[key] = GL_FALSE;
    }
}

int main()
{
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
        return 1;

#ifdef __APPLE__
    const char *glsl_version = "#version 150";
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#elif __linux__
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#elif _WIN32
    const char *glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#endif

#if __APPLE__
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, true);
#endif


    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT,
                                          "Untitled Game",
                                          nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(Settings::isVSync);

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // OpenGL configuration
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // order is important here, game constructor will call camera first.
    global::ui = std::make_unique<Ui>(window, glsl_version);
    global::camera = std::make_unique<Camera>(width, height);
    global::game = std::make_unique<Game>();

    float deltaTime{};
    float lastFrame{};

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame{ static_cast<float>(glfwGetTime()) };
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        global::game->processInput(deltaTime);
        global::game->update(deltaTime);

        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        global::game->render();
        global::ui->render();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glfwSwapBuffers(window);
    }

    global::game->destroy();
    ResourceManager::Clear();
    Ui::destroy();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
