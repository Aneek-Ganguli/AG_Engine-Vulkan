#include <vulkan/vulkan.hpp>

#include "Device.hpp"

#include <iostream>
#include <ostream>

Device::Device(vk::Instance &instance,vk::SurfaceKHR surface) {
    auto physicalDevices = instance.enumeratePhysicalDevices();
    if (physicalDevices.empty()) {
        throw std::runtime_error("No GPUs found!");
    }

    bool found = false;

    for (const auto& candidate : physicalDevices) {
        auto queueFamilies = candidate.getQueueFamilyProperties();

        for (uint32_t i = 0; i < queueFamilies.size(); ++i) {
            bool supportsGraphics =
                static_cast<bool>(queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics);

            bool supportsPresent =
                candidate.getSurfaceSupportKHR(i, surface);

            if (supportsGraphics && supportsPresent) {
                physicalDevice = candidate;
                queueFamilyIndex = i;
                found = true;
                break;
            }
        }

        if (found) break;
    }

    if (!found) {
        throw std::runtime_error("No suitable GPU found!");
    }


    float queuePriority = 1.0f;

    vk::DeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    vk::DeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

    device = physicalDevice.createDevice(deviceCreateInfo);
    queue = device.getQueue(queueFamilyIndex, 0);

}


void Device::destroy(vk::Instance &instance) {
    device.destroy();
}

