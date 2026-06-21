#ifndef PYTHONVK_COMMAND_BUFFER_H
#define PYTHONVK_COMMAND_BUFFER_H

#include <pythonvk/util/includes.h>
#include <pythonvk/command/command_pool.h>
#include <pythonvk/render/render_pass.h>
#include <pythonvk/render/framebuffer.h>
#include <pythonvk/render/graphics_pipeline.h>

class CommandBuffer {
    private:
        CommandPool* pool;
        LogicalDevice* device;

        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        VkCommandBufferAllocateInfo createInfo{};

        VkCommandBufferLevel level;

        void setCreateInfo();

    public:
        CommandBuffer(CommandPool* pool, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        ~CommandBuffer();

        inline CommandPool* getPool() { return pool; }
        inline VkCommandBuffer& getHandle() { return commandBuffer; }
        inline const VkCommandBufferAllocateInfo& getCreateInfo() const { return createInfo; }
        inline const VkCommandBufferLevel getLevel() const { return level; }

        void begin();
        void end();
        void beginRenderPass(RenderPass* renderPass, Framebuffer* framebuffer, std::vector<float> clearColor = {0.0f, 0.0f, 0.0f, 1.0f}, VkSubpassContents subpassContents = VK_SUBPASS_CONTENTS_INLINE);
        void endRenderPass();
        void reset();
        void bindPipeline(GraphicsPipeline* pipeline);
        void setViewport(float x, float y, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);
        void setScissor(int32_t x, int32_t y, uint32_t width, uint32_t height);
        void draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t firstVertex = 0, uint32_t firstInstance = 0);
};

#endif