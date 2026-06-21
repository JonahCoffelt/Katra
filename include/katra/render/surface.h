#ifndef KATRA_SURFACE_H
#define KATRA_SURFACE_H

// Includes
#include <katra/util/includes.h>
#include <katra/render/instance.h>
#include <katra/render/window.h>


// Class Declaration
class Surface {
    private:
        VkSurfaceKHR surface;
        Instance* instance;
        Window* window;

    public:
        Surface(Instance* instance, Window* window);
        ~Surface();
        Surface(const Surface&) = delete;
        Surface& operator=(const Surface&) = delete;

        inline VkSurfaceKHR getHandle() { return surface; }
        inline Window* getWindow() { return window; }
};

#endif