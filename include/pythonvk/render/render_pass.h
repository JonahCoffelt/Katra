#ifndef PYVK_RENDER_PASS_H
#define PYVK_RENDER_PASS_H

// Includes
#include <pythonvk/util/includes.h>
#include <pythonvk/device/logical_device.h>
#include <pythonvk/render/swap_chain.h>


// Class Declaration
class RenderPass {
    private:
        VkRenderPass renderPass = VK_NULL_HANDLE;
        LogicalDevice* device;
        SwapChain* swapChain;

        VkRenderPassCreateInfo renderPassCreateInfo;
        VkAttachmentDescription attachmentDescription;
        VkAttachmentReference attachmentReference;
        VkSubpassDescription subpassDescription;
        VkSubpassDependency subpassDependency;

        void setRenderPassCreateInfo();
        void setAttachmentDescription();
        void setAttachmentReference();
        void setSubpassDescription();
        void setSubpassDependency();

    public:
        RenderPass(LogicalDevice* device, SwapChain* swapChain);
        ~RenderPass();

        inline VkRenderPass getHandle() { return renderPass; }
        inline VkRenderPassCreateInfo getRenderPassCreateInfo() { return renderPassCreateInfo; }
        inline VkAttachmentDescription getAttachmentDescription() { return attachmentDescription; }
        inline VkAttachmentReference getAttachmentReference() { return attachmentReference; }
        inline VkSubpassDescription getSubpassDescription() { return subpassDescription; }
        inline VkSubpassDependency getSubpassDependency() { return subpassDependency; }
        inline LogicalDevice* getDevice() { return device; }
        inline SwapChain* getSwapChain() { return swapChain; }
};

#endif