#include <pythonvk/render/surface.h>

/**
 * @brief Construct a new Surface object for an existing instance and window
 * 
 * @param instance 
 * @param window 
 */
Surface::Surface(Instance* instance, Window* window): instance(instance), window(window) {
    VkResult result = glfwCreateWindowSurface(instance->getHandle(), window->getHandle(), nullptr, &surface);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

/**
 * @brief Destroy the vkSurface
 * 
 */
Surface::~Surface() {
    if (!surface) { return; }
    vkDestroySurfaceKHR(instance->getHandle(), surface, nullptr);
}