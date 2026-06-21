#ifndef KATRA_INSTANCE_H
#define KATRA_INSTANCE_H

// Includes
#include <katra/util/includes.h>
#include <katra/debug/debugger.h>
#include <katra/util/constants.h>


// Class Declaration
class Instance {
    private:
        VkInstance instance = VK_NULL_HANDLE;

        bool useValidation;
        std::string applicationName;
        VkApplicationInfo appInfo{};
        std::vector<const char*> extensions;
        VkInstanceCreateInfo createInfo{};
        VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{};
        uint32_t apiVersion;
        
        void setAppInfo(std::string name = "Katra", uint32_t apiVersion = VK_API_VERSION_1_0);
        void setRequiredExtensions();
        void setInstanceCreateInfo();
        
    public:
        Instance(std::string name = "Katra", bool useValidation = true, uint32_t apiVersion = VK_API_VERSION_1_0);
        ~Instance();

        inline VkInstance getHandle() { return instance; }
        inline uint32_t getApiVersion() const { return apiVersion; }
};

#endif