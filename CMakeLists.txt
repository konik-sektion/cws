cmake_minimum_required(VERSION 3.20)
project(meteo LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)

add_library(glad STATIC extern/glad/src/glad.c)
target_include_directory(glad PUBLIC extern/glad/include)

add_library(imgui STATIC
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/imgui_demo.cpp
    extern/imgui/backends/imgui_impl_glfw.cpp
    extern/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    extern/imgui
    extern/imgui/backends
)
target_link_libraries(imgui PUBLIC glfw glad)

add_executable(meteo
    src/main.cpp
    src/app/App.cpp
    src/render/GL.cpp
    src/render/Shader.cpp
    src/render/Texture2D.cpp
    src/render/Quad.cpp
    src/render/Viewport.cpp
    src/render/RasterLayout.cpp
    src/ui/Ui.cpp
)

target_include_directories(meteo PRIVATE src extern/glad/include extern/imgui extern/imgui/backends)

target_link_libraries(meteo PRIVATE glfw glad imgui)

if(UNIX AND NOT APPLE)
    target_link_libraries(meteo PRIVATE dl pthread)
endif()

add_custom_command(TARGET meteo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:meteo>/assets/shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/assets/shaders
            $<TARGET_FILE_DIR:meteo>/assets/shaders
)
