#ifndef PYVK_WINDOW_H
#define PYVK_WINDOW_H

// Includes
#include <pythonvk/util/includes.h>


// Class Declaration
class Window {
    private:
        GLFWwindow* window;

        std::string title;
        int width;
        int height;

    public:
        Window(std::string title, int width, int height);
        ~Window();

        inline GLFWwindow* getHandle() { return window; }
};

#endif