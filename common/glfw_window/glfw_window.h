#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class GLFWWindow {
  public:
    int         window_width;
    int         window_height;
    int         version_major;
    int         version_minor;
    std::string window_title;
    GLFWwindow* window;

    GLFWWindow(int window_width, int window_height, int version_major, int version_minor, std::string window_title);
    ~GLFWWindow();

    void create_window();
    int  validate_window();
};

#endif // GLFW_WINDOW_H