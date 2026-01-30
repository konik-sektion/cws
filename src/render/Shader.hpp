#pragma once
#include <string>
#include <glad/glad.h>

class Shader {
    public:
        Shader() = default;
        ~Shader();

        bool loadFromFiles(const std::string& vsPath, const std::string& fsPath);
        bool isValid() const;
        void bind() const;

        void setInt(const char* name, int v) const;
        void setFloat(const char* name, float v) const;

    private:
        GLuint prog_ = 0;

        static std::string readFile(const std::string& path);
        static GLuint compile(GLenum type, const std::string& src);
};
