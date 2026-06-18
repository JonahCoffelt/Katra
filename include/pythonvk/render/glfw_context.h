#ifndef PYVK_GLFW_CONTEXT_H
#define PYVK_GLFW_CONTEXT_H

// Includes
#include <pythonvk/util/includes.h>

// Struct Declaration
struct GLFWContext {
    GLFWContext() {
        if (!glfwInit()) throw std::runtime_error("failed to initialize GLFW!");
    }
    ~GLFWContext() { glfwTerminate(); }
};

#endif