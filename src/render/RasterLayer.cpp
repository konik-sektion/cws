#include "RasterLayer.hpp"
#include <algorithm>
#include <cstdio>
#include <vector>

RasterLayer::RasterLayer(int w, int h, const std::string& vs, const std::string& fs)
    : gridW_(w), gridH_(h)
{
    if (!shader_.loadFromFiles(vs, fs)) {
        std::fprintf(stderr, "Failed to load raster shader: %s %s\n", vs.c_str(), fs.c_str());
    }

    scalar_.createR32F(gridW_, gridH_);
    lut_.createLutRGBA8(256);
}

void RasterLayer::uploadScalar(const float* data) {
    scalar_.uploadR32F(data);
}

void RasterLayer::setParams(float vmin, float vmax, float alpha) {
    vmin_ = vmin;
    vmax_ = vmax;
    alpha_ = alpha;
}

static unsigned char u8(float x) {
    x = std::clamp(x, 0.0f, 1.0f);
    return (unsigned char)(x * 255.0f + 0.5f);
}

void RasterLayer::buildDefaultColormap() {
    const int N = 256;
    std::vector<unsigned char> rgba(N * 4);

    auto lerp = [](float a, float b, float t){ return a + (b - a)*t; };

    struct Stop { float t; float r, g, b; };
    Stop stops[] = {
        {0.00f, 0.05f, 0.10f, 0.35f}, // blue
        {0.20f, 0.00f, 0.70f, 0.95f}, // cyan 
        {0.40f, 0.00f, 0.80f, 0.20f}, // green
        {0.60f, 0.95f, 0.90f, 0.10f}, // yellow
        {0.75f, 0.95f, 0.25f, 0.05f}, // red
        {0.88f, 0.80f, 0.10f, 0.80f}, // magenta
        {1.00f, 0.98f, 0.98f, 0.98f}, // white
    };
    
    for (int i = 0; i < N; ++i) {
        float t = float(i) / float(N - 1);

        int k = 0;
        while (k + 1 < (int)(sizeof(stops)/sizeof(stops[0])) && t > stops[k + 1].t) k++;
        auto a = stops[k];
        auto b = stops[std::min(k + 1, (int)(sizeof(stops)/sizeof(stops[0])) - 1)];

        float local = (b.t > a.t) ? (t - a.t) / (b.t - a.t) : 0.0f;

        float r = lerp(a.r, b.r, local);
        float g = lerp(a.g, b.g, local);
        float bl = lerp(a.b, b.b, local);

        rgba[i * 4 + 0] = u8(r);
        rgba[i * 4 + 1] = u8(g);
        rgba[i * 4 + 2] = u8(bl);
        rgba[i * 4 + 3] = 255;
    }

    lut_.uploadLutRGBA8(rgba.data(), N);
}

void RasterLayer::render(int x, int y, int w, int h) {
    if (!shader_.isValid()) return;

    glViewport(x, y, w, h);
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, w, h);

    shader_.bind();

    scalar_.bind(0);
    lut_.bind(1);

    shader_.setInt("uScalar", 0);

    shader_.setInt("uLut", 1);
    shader_.setFloat("uVmin", vmin_);
    shader_.setFloat("uVmax", vmax_);
    shader_.setFloat("uAlpha", alpha_);

    quad_.draw();

    glDisable(GL_SCISSOR_TEST);
}
