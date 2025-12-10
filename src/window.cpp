#include <cassert>

#include <vulkan/vulkan.hpp>
#include <iostream>
#include  <Window.hpp>

void glfwErrorCallback(int error, const char* description){
    std::cerr << "[GLFW ERROR] (" << error << "): " << description << std::endl;
}


Window::Window(const char* p_title,int p_width,int p_height):title(p_title),width(p_width),height(p_height) {
    glfwSetErrorCallback(glfwErrorCallback);

    if (!glfwInit()) {
        std::cerr << "glfwInit failed. Life is pain." << std::endl;
        glfwTerminate();
        throw std::runtime_error("GLFW failed to initialize");
    }

    glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE , GLFW_FALSE);

    window = glfwCreateWindow(p_width,p_height,p_title,nullptr,nullptr);

    assert(window && "GLFW window creation failed!");


    uint32_t version = VK_MAKE_API_VERSION(0,1,0,0);
    vkEnumerateInstanceVersion(&version);


    vk::ApplicationInfo appInfo = vk::ApplicationInfo(title,VK_MAKE_VERSION(1, 0, 0),"AG_Engine Vulkan",
        VK_MAKE_VERSION(1, 0, 0),VK_API_VERSION_1_0);
    // appInfo.apiVersion = VK_API_VERSION_1_0;


    uint32_t glfwExtensionCount = 0;

    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    vk::InstanceCreateInfo createInfo{};
    createInfo.sType = vk::StructureType::eInstanceCreateInfo;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;




    // vk::createInstance()
    if (vk::createInstance(&createInfo, nullptr, &instance) != vk::Result::eSuccess) {
        throw std::runtime_error("failed to create instance!");
    }

    // std::cout << glfwExtensionCount << std::endl;

}

bool Window::isWindowOpen() {
    return !glfwWindowShouldClose(window);
}

void Window::cleanUp() {


    vkDestroyInstance(instance, nullptr);


    glfwDestroyWindow(window);

    glfwTerminate();
}



