#include <katra/synchronization/semaphore.h>

Semaphore::Semaphore(LogicalDevice* device): device(device) {
    setCreateInfo();
    VkResult result = vkCreateSemaphore(device->getHandle(), &createInfo, nullptr, &semaphore);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create semaphore!");
    }
}

void Semaphore::setCreateInfo() {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
}

Semaphore::~Semaphore() {
    if (semaphore != VK_NULL_HANDLE) {
        vkDestroySemaphore(device->getHandle(), semaphore, nullptr);
    }
}