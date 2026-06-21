#include <katra/render/shader_module.h>

/**
 * @brief Construct a new ShaderModule object
 * 
 */
ShaderModule::ShaderModule(LogicalDevice* device, const std::string& filename): device(device) {
    readFile(filename);
    setCreateInfo();

    // Create shader module
    VkResult result = vkCreateShaderModule(device->getHandle(), &createInfo, nullptr, &shaderModule);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
}

/**
 * @brief Set the create info
 * 
 */
void ShaderModule::setCreateInfo() {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
}

/**
 * @brief Read the file and store the code
 * 
 */
void ShaderModule::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    code.resize(fileSize);

    file.seekg(0);
    file.read(code.data(), fileSize);
    file.close();
}

/**
 * @brief Destroy the ShaderModule object
 * 
 */
ShaderModule::~ShaderModule() {
    if (shaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(device->getHandle(), shaderModule, nullptr);
    }
}