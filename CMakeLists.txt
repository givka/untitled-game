cmake_minimum_required(VERSION 3.14)
project(untilted-game)

file(GLOB helloworld_SRC
        "main.cpp"
        "src/*.h"
        "src/*.cpp"
        "includes/imgui/*.h"
        "includes/imgui/*.cpp"
        "includes/imgui/examples/imgui_impl_glfw.h"
        "includes/imgui/examples/imgui_impl_glfw.cpp"
        "includes/imgui/examples/imgui_impl_opengl3.h"
        "includes/imgui/examples/imgui_impl_opengl3.cpp"
        "includes/stb/*.h"
        )

include_directories(includes/imgui lib/SOIL/inc/SOIL includes/entt/single_include/)

add_executable(main ${helloworld_SRC} main.cpp src/game.cpp src/game.h src/shader.cpp src/shader.h src/texture.cpp src/texture.h src/resource_manager.cpp src/resource_manager.h src/sprite_renderer.cpp src/sprite_renderer.h src/settings.cpp src/settings.h src/camera.cpp src/camera.h src/global.h src/timer.h)

set_property(TARGET main PROPERTY CXX_STANDARD 17)
target_compile_options(main PRIVATE -Wall -Wextra)

# glfw
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE glfw)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE libglew_static)

# glm
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE glm)

# fmt
add_subdirectory(lib/fmt EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE fmt::fmt)

# SOIL
add_subdirectory(lib/SOIL EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE soil)

