#pragma once
#include "GLFW/glfw3.h"

class Device {
public:
    Device(vk::Instance& instance,vk::SurfaceKHR& surface,GLFWwindow* window);
    Device()=default;

    void destroy(vk::Instance& instance);

    vk::Device& getDevice(){return device;};

    void createSwapchain(vk::Instance &instance, vk::SurfaceKHR& surface,GLFWwindow* window);

private:
    uint32_t physicalDeviceCount{};
    vk::PhysicalDeviceProperties properties{};
    vk::PhysicalDevice physicalDevice{};
    vk::Device device{};
    vk::Queue queue{};
    vk::SwapchainKHR swapchain{};

    uint32_t queueFamilyIndex;
};

