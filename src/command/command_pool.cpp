#include <katra/command/command_pool.h>

CommandPool::CommandPool(LogicalDevice* device, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags): device(device), queueFamilyIndex(queueFamilyIndex), flags(flags) {
    setCreateInfo();

    VkResult result = vkCreateCommandPool(device->getHandle(), &createInfo, nullptr, &commandPool);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

CommandPool::~CommandPool() {
    vkDestroyCommandPool(device->getHandle(), commandPool, nullptr);
}

void CommandPool::setCreateInfo() {
    createInfo = {};
    createInfo.pNext = nullptr;
    createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    createInfo.flags = flags;
    createInfo.queueFamilyIndex = queueFamilyIndex;
}