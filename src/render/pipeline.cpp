#include <pythonvk/render/pipeline.h>

/**
 * @brief Construct a new Pipeline object
 * 
 */
Pipeline::Pipeline(LogicalDevice* device, RenderPass* renderPass): device(device), renderPass(renderPass) {
    // TODO: Implement
}

/**
 * @brief Destroy the Pipeline object
 * 
 */
Pipeline::~Pipeline() {
    if (pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(device->getHandle(), pipeline, nullptr);
    }
    if (pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(device->getHandle(), pipelineLayout, nullptr);
    }
}