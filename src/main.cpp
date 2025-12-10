#include  <iostream>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Window.hpp>


int main() {
    std::cout << "Hello World!" << std::endl;

    Window window("Hello In a British Accent",800,600);

    while (window.isWindowOpen()) {
        glfwPollEvents();
    }

    window.cleanUp();
    return 0;
}