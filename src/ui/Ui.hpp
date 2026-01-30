#define GLFW_INCLUDE_NONE

#pragma once
#include <GLFW/glfw3.h>
#include "../render/Viewport.hpp"

struct UiState {
    ViewportMode viewportMode = ViewportMode::Four;
    bool playing = true;
    float alpha = 0.85f;
    float vmin = 0.0f;
    float vmax = 1.0f;
};

void ui_init(GLFWwindow* window);
void ui_shutdown();
void ui_begin();
void ui_draw(UiState& s);
void ui_end();
