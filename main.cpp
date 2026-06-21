#include <pythonvk/pythonvk.h>

const std::vector<const char*> REQUIRED_DEVICE_EXTENSIONS = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
const std::vector<const char*> PREFERED_DEVICE_EXTENSIONS = {
    "VK_KHR_portability_subset"
};

bool isDeviceSuitable(PhysicalDevice& device) {
    if (!device.checkExtensionSupport(REQUIRED_DEVICE_EXTENSIONS)) {
        return false;
    }
    if (!device.supportsSwapChain()) {
        return false;
    }
    return device.hasGraphicsFamily() && device.hasPresentFamily();
}

class HelloTriangleApplication {
public:
    void run() {
        GLFWContext* context = new GLFWContext();
        initVulkan();
        mainLoop();
        cleanup();
        delete context;
    }

private:
    Window* window;
    Instance* instance;
    Debugger* debugger;
    Surface* surface;
    PhysicalDevice* physicalDevice;
    LogicalDevice* logicalDevice;
    SwapChain* swapChain;
    RenderPass* renderPass;
    GraphicsPipeline* graphicsPipeline;
    std::vector<Framebuffer*> swapChainFramebuffers;
    CommandPool* commandPool;
    std::vector<CommandBuffer*> commandBuffers;

    // Synchronization Objects
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    void initVulkan() {
        instance = new Instance();
        if (enableValidationLayers) { debugger = new Debugger(instance); }
        window = new Window("PythonVK", 800, 600);
        surface = new Surface(instance, window);

        pickPhysicalDevice();
        logicalDevice = new LogicalDevice(physicalDevice, REQUIRED_DEVICE_EXTENSIONS, PREFERED_DEVICE_EXTENSIONS);
        
        swapChain = new SwapChain(physicalDevice, logicalDevice, surface);
        renderPass = new RenderPass(logicalDevice, swapChain);
        graphicsPipeline = new GraphicsPipeline(logicalDevice, renderPass, "shaders/shader.vert.spv", "shaders/shader.frag.spv");
        createFramebuffers();

        commandPool = new CommandPool(logicalDevice, logicalDevice->getGraphicsFamilyIndex());
        
        createCommandBuffers();
        createSyncObjects();
    }

    void pickPhysicalDevice() {
        // Get all physical devices
        std::vector<PhysicalDevice> physicalDevices = getPhysicalDevices(instance, surface);
        
        // Find device with best score
        unsigned int bestScore = 0;
        PhysicalDevice* bestDevice = nullptr;
        for (auto& device : physicalDevices) {
            if (!isDeviceSuitable(device)) continue;
            if (!bestDevice || device.getScore() > bestScore) {
                bestScore = device.getScore();
                bestDevice = &device;
            }
        }
        
        // If there is no suitable device, you lowk fucked and poor as well
        if (!bestDevice) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        // Create physical device object from best device
        physicalDevice = new PhysicalDevice(bestDevice->getHandle(), surface);
        std::cout << "Using device: " << physicalDevice->getName() << std::endl;
    }

    void createFramebuffers() {
        // Loop through images and create a framebuffer
        for (size_t i = 0; i < swapChain->getImageCount(); i++) {
            std::vector<VkImageView> attachments = { swapChain->getImageViews()[i] };
            Framebuffer* framebuffer = new Framebuffer(logicalDevice, renderPass, attachments, 1);
            swapChainFramebuffers.push_back(framebuffer);
        }
    }

    void createCommandBuffers() {
        // Make buffers for each in flight frame
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            commandBuffers[i] = new CommandBuffer(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        }
    }

    void recordCommandBuffer(CommandBuffer* commandBuffer, uint32_t imageIndex) {
        commandBuffer->begin();
        commandBuffer->beginRenderPass(renderPass, swapChainFramebuffers[imageIndex], {0.0f, 0.0f, 0.0f, 1.0f});
        commandBuffer->bindPipeline(graphicsPipeline);
        commandBuffer->setViewport(0.0f, 0.0f, static_cast<float>(swapChain->getExtent().width), static_cast<float>(swapChain->getExtent().height));
        commandBuffer->setScissor(0, 0, swapChain->getExtent().width, swapChain->getExtent().height);
        commandBuffer->draw(3, 1, 0, 0);
        commandBuffer->endRenderPass();
        commandBuffer->end();
    }

    void createSyncObjects() {

        // We want sync objects for each in frame
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(swapChain->getImageCount()); // need one for every image

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;  // Allows the first frame to fall through

        // Create MAX_FRAMES_IN_FLIGHT imageAvailableSemaphores and inFlightFences
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(logicalDevice->getHandle(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(logicalDevice->getHandle(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create sync objects!");
            }
        }
        // Create swapChainImages.size() renderFinishedSemaphores
        for (size_t i = 0; i < swapChain->getImageCount(); i++) {
            if (vkCreateSemaphore(logicalDevice->getHandle(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create renderFinished semaphores!");
            }
        }
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window->getHandle())) {
            glfwPollEvents();
            drawFrame();
        }

        logicalDevice->waitIdle();
    }

    void drawFrame() {
        // Make sure no other frame is rendering
        vkWaitForFences(logicalDevice->getHandle(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
        // Reset fence
        vkResetFences(logicalDevice->getHandle(), 1, &inFlightFences[currentFrame]);

        // Aquire Image from swap chain
        uint32_t imageIndex;
        vkAcquireNextImageKHR(logicalDevice->getHandle(), swapChain->getHandle(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        // Record the command buffer
        commandBuffers[currentFrame]->reset();
        recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

        // Submit the command buffer
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        // Semaphores to wait on (wait for image to be available)
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.waitSemaphoreCount = 1;
        // Use color attachment
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[currentFrame]->getHandle();
        // Signal the render finished semaphore when complete
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[imageIndex]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        // Use inFlightFence so the next frame will wait on this command queue
        VkResult result = vkQueueSubmit(logicalDevice->getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        // Presentation
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = {swapChain->getHandle()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        vkQueuePresentKHR(logicalDevice->getPresentQueue(), &presentInfo);

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void cleanup() {
        // If validation is enabled, destory the messenger
        if (enableValidationLayers) {
            delete debugger;
        }

        // Destroy synchronization objects
        for (size_t i = 0; i < swapChain->getImageCount(); i++) {
            vkDestroySemaphore(logicalDevice->getHandle(), renderFinishedSemaphores[i], nullptr);
        }
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(logicalDevice->getHandle(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(logicalDevice->getHandle(), inFlightFences[i], nullptr);
        }
        
        // Destroy all framebuffer (must happen before image pipeline destroy)
        for (auto framebuffer : swapChainFramebuffers) {
            delete framebuffer;
        }
        
        for (auto commandBuffer : commandBuffers) {
            delete commandBuffer;
        }
        
        delete commandPool;
        delete graphicsPipeline;
        delete renderPass;
        delete swapChain;
        delete logicalDevice;
        delete physicalDevice;
        delete surface;
        delete window;
        delete instance;
    }
};

int main() {
    HelloTriangleApplication app;
    app.run();
}