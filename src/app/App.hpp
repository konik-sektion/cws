#pragma once

struct GLFWwindow;

class App {
    public:
        bool init();
        bool run();
        void shutdown();

    private:
        GLFWwindow* window_ = nullptr;
        int fbW_ = 1280;
        int fbH_ = 720;
};
