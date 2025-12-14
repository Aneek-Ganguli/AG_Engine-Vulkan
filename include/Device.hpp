#pragma once

class Device {
public:
    Device(vk::Instance& instance,vk::SurfaceKHR surface);
    Device(){}

    void destroy(vk::Instance& instance);

private:
    uint32_t physicalDeviceCount{};
    vk::PhysicalDeviceProperties properties;
    vk::PhysicalDevice physicalDevice;
    vk::Device device;
    vk::Queue queue;

    uint32_t queueFamilyIndex;
};

