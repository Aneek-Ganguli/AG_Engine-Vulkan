#include <cassert>

#include <vulkan/vulkan.hpp>
#include <iostream>
#include  <Window.hpp>


void glfwErrorCallback(int error, const char* description){
    std::cerr << "[GLFW ERROR] (" << error << "): " << description << std::endl;
}

bool supported(std::vector<const char*>& p_extensions,std::vector<const char *>& p_layers) {

    std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();

    if (debug){
        std::cout << "Supported extensions: " << std::endl;
        for (vk::ExtensionProperties supportedExtension : supportedExtensions) {
            std::cout << supportedExtension.extensionName << std::endl;
        }
    }

    bool found;
    for (const char* extension : p_extensions) {
        found = false;
        for (vk::ExtensionProperties supportedExtension : supportedExtensions) {
            if (strcmp(extension, supportedExtension.extensionName) == 0) {
                found = true;
                if (debug)std::cout << "Extension \"" << extension << "\" is supported!\n";
            }
        }
        if (!found) {
            if (debug){throw std::runtime_error("Extension  is not supported!\n");}
            return false;
        }
    }

    std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();

    if (debug){
        std::cout << "Device can support the following layers:\n";
        for (vk::LayerProperties supportedLayer : supportedLayers) {
            std::cout << '\t' << supportedLayer.layerName << '\n';
        }
    }


    for (const char* layer : p_layers) {
        found = false;
        for (vk::LayerProperties supportedLayer : supportedLayers) {
            if (strcmp(layer, supportedLayer.layerName) == 0) {
                found = true;
                if (debug)std::cout << "Layer \"" << layer << "\" is supported!\n";
            }
        }
        if (!found) {
                if (debug)std::cout << "Layer \"" << layer << "\" is not supported!\n";
            return false;
        }
    }

    return true;
}


Window::Window(const char* p_title,int p_width,int p_height):title(p_title),width(p_width),height(p_height),logger() {

    createGLFWwindow();

    uint32_t version = VK_MAKE_API_VERSION(0,1,0,0);
    vkEnumerateInstanceVersion(&version);


    vk::ApplicationInfo appInfo = vk::ApplicationInfo(title,VK_MAKE_VERSION(1, 0, 0),"AG_Engine Vulkan",
        VK_MAKE_VERSION(1, 0, 0),VK_API_VERSION_1_0);
    // appInfo.apiVersion = VK_API_VERSION_1_0;

    //create glfw extensions
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions,glfwExtensions+glfwExtensionCount);

    for (const char* e : extensions) {
        std::cout << e << "\n" ;
    }
    std::vector<const char*> layers = {"VK_LAYER_KHRONOS_validation"};

    {
        extensions.push_back("VK_EXT_debug_utils");
    }

    // debug = true;
    if (!supported(extensions,layers)) {
        throw std::runtime_error("Either Validation Layers or GLFW Extensions arent supported");
    }
    vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(),&appInfo,layers.size(),layers.data(),
        extensions.size(),extensions.data());
    // createInfo.sType = vk::StructureType::eInstanceCreateInfo;
    // createInfo.pApplicationInfo = &appInfo;
    // createInfo.enabledExtensionCount = glfwExtensionCount;
    // createInfo.ppEnabledExtensionNames = glfwExtensions;


    // std::cout << glfwExtensions <<std::endl;

    // debug = true;

    // vk::createInstance()
    if (vk::createInstance(&createInfo, nullptr, &instance) != vk::Result::eSuccess) {
        throw std::runtime_error("failed to create instance!");
    }

    // std::cout << glfwExtensionCount << std::endl;

    logger = Logger(instance);

}

bool Window::isWindowOpen() {
    return !glfwWindowShouldClose(window);
}

void Window::cleanUp() {
    logger.cleanUp(instance);
    vkDestroyInstance(instance, nullptr);

    glfwDestroyWindow(window);

    glfwTerminate();
}

void Window::createGLFWwindow() {
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "glfwInit failed. Life is pain." << std::endl;
        glfwTerminate();
        throw std::runtime_error("GLFW failed to initialize");
    }

    glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE , GLFW_FALSE);

    window = glfwCreateWindow(width,height,title,nullptr,nullptr);

    assert(window && "GLFW window creation failed!");
}

