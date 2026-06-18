#include <pythonvk/render/instance.h>

/**
 * @brief Construct a new Instance object
 * 
 * @param useValidation Enable validation layers
 */
Instance::Instance(
    std::string name, 
    bool useValidation, 
    unsigned int variant, 
    unsigned int major_version, 
    unsigned int minor_version, 
    unsigned int patch): 
        useValidation(useValidation) 
    {

    // Set app info and extensions
    setAppInfo(name, variant, major_version, minor_version, patch);
    setRequiredExtensions();
    setInstanceCreateInfo();

    // Create the Vulkan instance
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

/**
 * @brief Set the instance creation info from app info and extensions
 * 
 */
void Instance::setInstanceCreateInfo() {
    createInfo = {};
    
    // General info
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;

    // Use validation layers if enabled
    if (useValidation) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();

        debugMessengerCreateInfo = Debugger::getDebugMessengerCreateInfo();
        createInfo.pNext = &debugMessengerCreateInfo;
    }

    // Mac Compatibility
    #ifdef __APPLE__
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif
}

/**
 * @brief Set the app info, containing name and version
 * 
 * @param name 
 * @param variant 
 * @param major_version 
 * @param minor_version 
 * @param patch 
 */
void Instance::setAppInfo(std::string name, unsigned int variant, unsigned int major_version, unsigned int minor_version, unsigned int patch) {
    applicationName = std::move(name);
    appInfo = {};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName.c_str();
    appInfo.pEngineName = "PythonVK";
    uint32_t version = VK_MAKE_API_VERSION(variant, major_version, minor_version, patch);
    appInfo.applicationVersion = version;
    appInfo.engineVersion = version;
    appInfo.apiVersion = version;
}

/**
 * @brief Set the required extensions for glfw (must use glfw)
 * 
 */
void Instance::setRequiredExtensions() {
    extensions.clear();

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    extensions.assign(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (useValidation) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    #ifdef __APPLE__
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    #endif
} 

/**
 * @brief Destroy the vkInstance
 * 
 */
Instance::~Instance() {
    if (!instance) { return; }
    vkDestroyInstance(instance, nullptr);
}