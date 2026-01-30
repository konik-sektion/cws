#define GLFW_INCLUDE_NONE

#include "App.hpp"

#include <cstdio>
#include <vector>
#include <cmath>
#include <algorithm>

#include <GLFW/glfw3.h>

#include "../render/GL.hpp"
#include "../render/Viewport.hpp"
#include "../render/RasterLayer.hpp"
#include "../ui/Ui.hpp"

static void glfw_error_cb(int, const char* desc) {
    std::fprintf(stderr, "GLFW error: %s\n", desc);
}

bool App::init() {
    glfwSetErrorCallback(glfw_error_cb);
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow(1280, 720, "meteo", nullptr, nullptr);
    if (!window_) return false;

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    if (!gl_init()) return false;

    ui_init(window_);
    return true;
}

void App::run() {
    const int gridW = 512, gridH = 256;

    RasterLayer raster(gridW, gridH, "assets/shaders/blit.vert", "assets/shaders/scalar.frag");
    raster.buildDefaultColormap();

    ViewportLayout layout;
    layout.mode = ViewportMode::Four;

    UiState ui{};
    ui.viewportMode = layout.mode;
    ui.playing = true;
    ui.alpha = 0.85f;
    ui.vmin = 0.0f;
    ui.vmax = 1.0f;

    float t = 0.0f;
    std::vector<float> field(gridW * gridH);

    while (!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        glfwGetFramebufferSize(window_, &fbW_, &fbH_);

        if (ui.playing) t += 0.016f;

        // Dummy scalar field (you’ll swap this with real gridded data later)
        for (int y = 0; y < gridH; ++y) {
            for (int x = 0; x < gridW; ++x) {
                float fx = float(x) / float(gridW);
                float fy = float(y) / float(gridH);

                float v =
                    0.55f
                    + 0.25f * std::sin(6.0f * fx + t)
                    + 0.20f * std::cos(5.0f * fy - 0.8f * t)
                    + 0.10f * std::sin(10.0f * (fx + fy) + 0.4f * t);

                field[y * gridW + x] = v;
            }
        }

        raster.uploadScalar(field.data());
        raster.setParams(ui.vmin, ui.vmax, ui.alpha);

        glClearColor(0.06f, 0.06f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        layout.mode = ui.viewportMode;
        auto rects = layout.computeRects(fbW_, fbH_);

        for (const auto& r : rects) {
            raster.render(r.x, r.y, r.w, r.h);
        }

        ui_begin();
        ui_draw(ui);
        ui_end();

        // Clamp sanity
        ui.vmax = std::max(ui.vmax, ui.vmin + 1e-6f);

        glfwSwapBuffers(window_);
    }
    return true;
}

void App::shutdown() {
    ui_shutdown();
    if (window_) glfwDestroyWindow(window_);
    glfwTerminate();
}
