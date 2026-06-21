#include <katra/render/window.h>

/**
 * @brief Construct a new Window object and initialize glfw
 * 
 * @param title 
 * @param width 
 * @param height 
 */
Window::Window(std::string title, int width, int height): title(title), width(width), height(height) {
    // Initialize GLFW
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    
    // Create the window
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("failed to create window!");
    }
}

/**
 * @brief Destroy the glfwWindow object and terminate glfw
 * 
 */
Window::~Window() {
    glfwDestroyWindow(window);
}