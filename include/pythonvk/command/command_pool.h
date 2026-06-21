#ifndef PYTHONVK_COMMAND_POOL_H
#define PYTHONVK_COMMAND_POOL_H

#include <pythonvk/util/includes.h>
#include <pythonvk/device/logical_device.h>

class CommandPool {
    private:
        LogicalDevice* device;

        VkCommandPool commandPool = VK_NULL_HANDLE;
        VkCommandPoolCreateInfo createInfo{};

        uint32_t queueFamilyIndex;
        VkCommandPoolCreateFlags flags;

        void setCreateInfo();

    public:
        CommandPool(LogicalDevice* device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
        ~CommandPool();

        inline VkCommandPool& getHandle() { return commandPool; }
        inline const VkCommandPoolCreateInfo& getCreateInfo() const { return createInfo; }
        inline LogicalDevice* getDevice() { return device; }
};

#endif