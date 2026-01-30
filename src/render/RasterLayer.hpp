#pragma once
#include <string>
#include "Shader.hpp"
#include "Texture2D.hpp"
#include "Quad.hpp"

class RasterLayer {
    public:
        RasterLayer(int w, int h, const std::string& vs, const std::string& fs);

        void uploadScalar(const float* data);
        void buildDefaultColormap();
        void setParams(float vmin, float vmax, float alpha);

        void render(int x, int y, int w, int h);

    private:
        Shader shader_;
        Texture2D scalar_;
        Texture2D lut_;
        Quad quad_;

        float vmin_ = 0.0f;
        float vmax_ = 1.0f;
        float alpha_ = 1.0f;

        int gridW_ = 0, gridH_ = 0;
};
