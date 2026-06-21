#ifndef KATRA_GLFW_CONTEXT_H
#define KATRA_GLFW_CONTEXT_H

// Includes
#include <katra/util/includes.h>

// Struct Declaration
struct GLFWContext {
    GLFWContext() {
        if (!glfwInit()) throw std::runtime_error("failed to initialize GLFW!");
    }
    ~GLFWContext() { glfwTerminate(); }
};

#endif