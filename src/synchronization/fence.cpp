#include <katra/synchronization/fence.h>

/**
 * @brief Construct a new Fence:: Fence object
 * 
 * @param device Logical device
 * @param signaled If the fence should be created in a signaled state
 */
Fence::Fence(LogicalDevice* device, bool signaled): device(device) {
    setCreateInfo(signaled);
    VkResult result = vkCreateFence(device->getHandle(), &createInfo, nullptr, &fence);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create fence!");
    }
}

/**
 * @brief Set the create info for the fence
 * 
 * @param signaled If the fence should be created in a signaled state
 */
void Fence::setCreateInfo(bool signaled) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
}

void Fence::wait(uint64_t timeout) {
    vkWaitForFences(device->getHandle(), 1, &fence, VK_TRUE, timeout);
}

void Fence::reset() {
    vkResetFences(device->getHandle(), 1, &fence);
}

/**
 * @brief Destroy the Fence object
 * 
 */
Fence::~Fence() {
    if (fence != VK_NULL_HANDLE) {
        vkDestroyFence(device->getHandle(), fence, nullptr);
    }
}