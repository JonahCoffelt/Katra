#ifndef PYVK_SURFACE_H
#define PYVK_SURFACE_H

// Includes
#include <pythonvk/util/includes.h>
#include <pythonvk/render/instance.h>
#include <pythonvk/render/window.h>


// Class Declaration
class Surface {
    private:
        VkSurfaceKHR surface;
        Instance* instance;
        Window* window;

    public:
        Surface(Instance* instance, Window* window);
        ~Surface();

        inline VkSurfaceKHR getHandle() { return surface; }
        inline Window* getWindow() { return window; }
};

#endif