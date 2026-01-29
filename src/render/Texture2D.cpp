#include "Texture2D.hpp"

Texture2D::~Texture2D() {
    if (id_) glDeleteTextures(1, &id_);
}

void Texture2D::createR32F(int w, int h) {
    w_ = w; h_ = h;
    target_ = GL_TEXTURE_2D;

    if (id_) glGenTexture(target_, id_);
    glBindTexture(target_, id_);

    glTexImage2D(target_, 0, GL_R32F, w_, h_, 0, GL_RED, GL_FLOAT, nullptr);

    glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture2D::createLutRGBA8(int w) {
    w_ = w; h_ = 1;
    target_ = GL_TEXTURE_2D;

    if (!id_) glGenTextures(1, &id_);
    glBindTexture(target_, id_);

    glTexImage2D(target_, 0, GL_RGBA8, w_, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target_, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target_, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture2D::uploadR32F(const float* data) {
    glBindTexture(target_, id_);
    glTexSubImage2D(target_, 0, 0, 0, w_, h_, GL_RED, GL_FLOAT, data);
}

void Texture2D::uploadLutRGBA8(const unsigned char* rgba, int w) {
    glBindTexture(target_, id_);
    glTexSubImage2D(target_, 0, 0, 0, w, 1, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
}

void Texture2D::bind(int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(target_, id_);
}
