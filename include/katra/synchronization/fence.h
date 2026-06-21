#ifndef KATRA_FENCE_H
#define KATRA_FENCE_H

// Includes
#include <katra/util/includes.h>
#include <katra/device/logical_device.h>

// Class Declaration
class Fence {
    private:
        LogicalDevice* device;

        VkFence fence = VK_NULL_HANDLE;

        VkFenceCreateInfo createInfo{};

        void setCreateInfo(bool signaled);

    public:
        Fence(LogicalDevice* device, bool signaled = false);
        ~Fence();
        Fence(const Fence&) = delete;
        Fence& operator=(const Fence&) = delete;

        inline VkFence getHandle() { return fence; }
        inline const VkFenceCreateInfo& getCreateInfo() const { return createInfo; }

        void wait(uint64_t timeout = UINT64_MAX);
        void reset();
};

#endif