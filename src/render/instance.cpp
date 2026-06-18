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

    // Get app info and extensions
    VkApplicationInfo appInfo = getAppInfo(name, variant, major_version, minor_version, patch);
    std::vector<const char*> extensions = getRequiredExtensions();

    // Create the Vulkan instance
    VkInstanceCreateInfo createInfo = getInstanceCreateInfo(appInfo, extensions);
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

/**
 * @brief Get the instance creation info from app info and extensions
 * 
 * @param appInfo 
 * @param extensions 
 * @return VkInstanceCreateInfo 
 */
VkInstanceCreateInfo Instance::getInstanceCreateInfo(VkApplicationInfo& appInfo, std::vector<const char*>& extensions) {
    VkInstanceCreateInfo createInfo{};
    
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

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = Debugger::getDebugMessengerCreateInfo();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }

    // Mac Compatibility
    #ifdef __APPLE__
        createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    #endif

    return createInfo;
}

/**
 * @brief Get the app info, containing name and version
 * 
 * @param name 
 * @param variant 
 * @param major_version 
 * @param minor_version 
 * @param patch 
 * @return VkApplicationInfo 
 */
VkApplicationInfo Instance::getAppInfo(std::string name, unsigned int variant, unsigned int major_version, unsigned int minor_version, unsigned int patch) {
    VkApplicationInfo appInfo{};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    // Names
    appInfo.pApplicationName = name.data();
    appInfo.pEngineName = "PythonVK";
    // Versions
    uint32_t version = VK_MAKE_API_VERSION(variant, major_version, minor_version, patch);
    appInfo.applicationVersion = version;
    appInfo.engineVersion = version;
    appInfo.apiVersion = version;

    return appInfo;
}

/**
 * @brief Get the required extensions for glfw (must use glfw)
 * 
 * @return std::vector<const char*>
 */
std::vector<const char*> Instance::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (useValidation) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    #ifdef __APPLE__
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    #endif

    return extensions;
} 

/**
 * @brief Destroy the vkInstance
 * 
 */
Instance::~Instance() {
    if (!instance) { return; }
    vkDestroyInstance(instance, nullptr);
}