#include <vulkan/vulkan.hpp>

#include "Device.hpp"

#include <iostream>
#include <ostream>

#include "GLFW/glfw3.h"

Device::Device(vk::Instance &instance,vk::SurfaceKHR& surface,GLFWwindow* window) {
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

    const char* extensions[] = {
        vk::KHRSwapchainExtensionName
    };

    vk::DeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    vk::DeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.enabledExtensionCount = sizeof(extensions) / sizeof(extensions[0]);
    deviceCreateInfo.ppEnabledExtensionNames = extensions;

    device = physicalDevice.createDevice(deviceCreateInfo);

    queue = device.getQueue(queueFamilyIndex, 0);


    createSwapchain(instance,surface,window);
}


void Device::destroy(vk::Instance &instance) {
    device.destroySwapchainKHR(swapchain);
    device.destroy();
}
void Device::createSwapchain(vk::Instance &instance,vk::SurfaceKHR& surface,GLFWwindow* window) {
    // Surface capabilities
    vk::SurfaceCapabilitiesKHR surfaceCaps = physicalDevice.getSurfaceCapabilitiesKHR(surface);

    // Image count
    uint32_t imageCount = std::max(3u, surfaceCaps.minImageCount);
    if (surfaceCaps.maxImageCount > 0) {
        imageCount = std::min(imageCount, surfaceCaps.maxImageCount);
    }

    // Surface formats
    std::vector<vk::SurfaceFormatKHR> surfaceFormats =
        physicalDevice.getSurfaceFormatsKHR(surface);

    // Pick preferred format
    vk::SurfaceFormatKHR surfaceFormat = surfaceFormats.front();
    for (const auto& candidate : surfaceFormats) {
        if (candidate.format == vk::Format::eB8G8R8A8Srgb &&
            candidate.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            surfaceFormat = candidate;
            break;
            }
    }

    // Framebuffer size
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);

    auto presentMode = physicalDevice.getSurfacePresentModesKHR(surface);
    if (presentMode.empty()) {
        throw std::runtime_error("No present mode found!");
    }

    vk::PresentModeKHR actualPresentMode = vk::PresentModeKHR::eFifo;
    for (auto e : presentMode) {
        if (e == vk::PresentModeKHR::eMailbox) {
            actualPresentMode = e;
            break;
        }
    }


    // Swapchain create info
    vk::SwapchainCreateInfoKHR swapchainCI{};
    swapchainCI.surface = surface;
    swapchainCI.minImageCount = imageCount;
    swapchainCI.imageFormat = surfaceFormat.format;
    swapchainCI.imageColorSpace = surfaceFormat.colorSpace;
    swapchainCI.imageExtent = vk::Extent2D{
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height)
    };
    swapchainCI.imageArrayLayers = 1;
    swapchainCI.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
    swapchainCI.preTransform = surfaceCaps.currentTransform;
    swapchainCI.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
    swapchainCI.presentMode = actualPresentMode;
    swapchainCI.clipped = VK_TRUE;

    // Create swapchain
    swapchain = device.createSwapchainKHR(swapchainCI);
    if (swapchain == nullptr) {
        throw std::runtime_error("Failed to create swapchain!");
    }
}

