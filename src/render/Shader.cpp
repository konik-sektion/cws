#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <cstdio>

Shader::~Shader() {
    if (prog_) glDeleteProgram(prog_);
}

std::string Shader::readFile(const std::string& path) {
    std::ifstream f(path);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

GLuint Shader::compile(GLenum type, const std::string& src) {
    GLuint s = glCreateShader(type);
    const char* c = src.c_str();
    glShaderSource(s, 1, &c, nullptr);
    glCompileShader(s);

    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[4096];
        glGetShaderInfoLog(s, sizeof(log), nullptr, log);
        std::fprintf(stderr, "Shader compile error: %s\n", log);
        glDeleteShader(s);
        return 0;
    }
    return s;
}

bool Shader::loadFromFiles(const std::string& vsPath, const std::string& fsPath) {
    auto vsSrc = readFile(vsPath);
    auto fsSrc = readFile(fsPath);
    if (vsSrc.empty() || fsSrc.empty()) {
        std::fprintf(stderr, "Failed to read shader files: %s %s\n", vsPath.c_str(), fsPath.c_str());
        return false;
    }

    GLuint vs = compile(GL_VERTEX_SHADER, vsSrc);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fsSrc);
    if (!vs || !fs) return false;

    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);

    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint ok = 0;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[4096];
        glGetProgramInfoLog(p, sizeof(log), nullptr, log);
        std::fprintf(stderr, "Program link error: %s\n", log);
        glDeleteProgram(p);
        return false;
    }

    if (prog_) glDeleteProgram(prog_);
    prog_ = p;
    return true;
}

void Shader::bind() const { glUseProgram(prog_); }

void Shader::setInt(const char* name, int v) const {
    glUniform1i(glGetUniformLocation(prog_, name), v);
}

void Shader::setFloat(const char* name, float v) const {
    glUniform1f(glGetUniformLocation(prog_, name), v);
}
