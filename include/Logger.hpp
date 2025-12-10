#pragma once
#include <vulkan/vulkan.hpp>

class Logger {
    public:
    Logger(vk::Instance& instance);
    Logger(){};
    void cleanUp(vk::Instance& instance);


private:
    vk::DebugUtilsMessengerEXT debugMessenger{nullptr};
    vk::detail::DispatchLoaderDynamic dldi{};

    vk::DebugUtilsMessengerEXT createMessenger(vk::Instance &instance);
};

