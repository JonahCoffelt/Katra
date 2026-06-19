#ifndef PYVK_PIPELINE_H
#define PYVK_PIPELINE_H

// Includes
#include <pythonvk/util/includes.h>
#include <pythonvk/device/logical_device.h>


// Class Declaration
class Pipeline {
    private:
        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

        LogicalDevice* device;

    public:
        Pipeline(LogicalDevice* device);
        ~Pipeline();
};

#endif