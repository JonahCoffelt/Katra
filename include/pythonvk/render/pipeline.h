#ifndef PYVK_PIPELINE_H
#define PYVK_PIPELINE_H

// Includes
#include <pythonvk/util/includes.h>
#include <pythonvk/device/logical_device.h>
#include <pythonvk/render/render_pass.h>

// Class Declaration
class Pipeline {
    private:
        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

        LogicalDevice* device;
        RenderPass* renderPass;

    public:
        Pipeline(LogicalDevice* device, RenderPass* renderPass);
        ~Pipeline();
};

#endif