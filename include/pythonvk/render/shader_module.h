#ifndef PYVK_SHADER_MODULE_H
#define PYVK_SHADER_MODULE_H

// Includes
#include <pythonvk/util/includes.h>
#include <pythonvk/device/logical_device.h>


// Class Declaration
class ShaderModule {
    private:
        VkShaderModule shaderModule = VK_NULL_HANDLE;
        LogicalDevice* device;

        VkShaderModuleCreateInfo createInfo{};
        std::vector<char> code;

        void setCreateInfo();
        void readFile(const std::string& filename);
    
    public:
        ShaderModule(LogicalDevice* device, const std::string& filename);
        ~ShaderModule();

        inline VkShaderModule getHandle() { return shaderModule; }
        inline LogicalDevice* getDevice() { return device; }
        inline VkShaderModuleCreateInfo getCreateInfo() { return createInfo; }

};

#endif