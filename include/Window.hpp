#pragma once
#include <GLFW/glfw3.h>

#include  <vulkan/vulkan.hpp>

#include "Logger.hpp"

#ifdef DEBUG
    static bool debug = true;
#else
    static bool debug = false;
#endif

class Window {
public:
    Window(const char *p_title, int width, int p_height);
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool isWindowOpen();
    void cleanUp();

private:
    const char* title;
    int width, height;
    GLFWwindow* window{};
    Logger logger;
    vk::Instance instance{};

    void createGLFWwindow();
};
