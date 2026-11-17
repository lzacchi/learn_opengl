#include <glad/glad.h>
#include <iostream>

#include "glfw_window.h"

GLFWWindow::GLFWWindow(int width, int height, int opengl_major,
                       int opengl_minor, std::string title)
    : window_width(width), window_height(height),
      version_major(opengl_major),
      version_minor(opengl_minor), window_title(title), window(nullptr) {

    create_window();
}

GLFWWindow::~GLFWWindow() {
    glfwTerminate();
}

void GLFWWindow::create_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(window_width, window_height, window_title.c_str(), NULL, NULL);
}

int GLFWWindow::validate_window() {
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }
    return 0;
}