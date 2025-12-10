#pragma once
#include <GLFW/glfw3.h>
#include  <vulkan/vulkan.hpp>


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
    vk::Instance instance{};
};
