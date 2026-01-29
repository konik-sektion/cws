#pragma once
#include <glad/glad.h>

class Texture2D {
    public:
        Texture2D() = default;
        ~Texture2D();

        void createR32F(int w, int h);
        void createLutRGBA8(int w);
        void uploadR32F(const float* data);
        void uploadLutRGBA8(const unsigned char* rgba, int w);

        void bind(int unit) const;

        int w() const { return w_; }
        int h() const { return h_; }

    private:
        GLuint id_ = 0;
        GLenum target_ = GL_TEXTURE_2D;
        int w_ = 0, h_ = 0;
};
