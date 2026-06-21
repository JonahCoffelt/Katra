#ifndef KATRA_SEMAPHORE_H
#define KATRA_SEMAPHORE_H

// Includes
#include <katra/util/includes.h>
#include <katra/device/logical_device.h>


// Class Declaration
class Semaphore {
    private:
        LogicalDevice* device;

        VkSemaphore semaphore = VK_NULL_HANDLE;

        VkSemaphoreCreateInfo createInfo{};

        void setCreateInfo();

    public:
        Semaphore(LogicalDevice* device);
        ~Semaphore();
        Semaphore(const Semaphore&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;

        inline VkSemaphore getHandle() { return semaphore; }
        inline const VkSemaphoreCreateInfo& getCreateInfo() const { return createInfo; }
};

#endif