#ifndef PYVK_INSTANCE_H
#define PYVK_INSTANCE_H

// Includes
#include <pythonvk/util/includes.h>
#include <pythonvk/debug/debugger.h>
#include <pythonvk/util/constants.h>


// Class Declaration
class Instance {
    private:
        VkInstance instance;
        bool useValidation;

        VkApplicationInfo getAppInfo(std::string name = "PythonVK", unsigned int variant = 0, unsigned int major_version = 1, unsigned int minor_version = 0, unsigned int patch = 0);
        std::vector<const char*> getRequiredExtensions();
        VkInstanceCreateInfo getInstanceCreateInfo(VkApplicationInfo& appInfo, std::vector<const char*>& extensions);
        
    public:
        Instance(std::string name = "PythonVK", bool useValidation = true, unsigned int variant = 0, unsigned int major_version = 1, unsigned int minor_version = 0, unsigned int patch = 0);
        ~Instance();

        inline VkInstance getHandle() { return instance; }

};

#endif