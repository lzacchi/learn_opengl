#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#include <shader/shader.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

const int WINDOW_WIDTH = 1400;
const int WINDOW_HEIGHT = 900;

int main() {
    /* In the main function, we initialize GLFW with glfwInit, and after that we configure it
     * using glfwWindowHint. Its first argument indicates what option we want to select, based
     * on a large enum of possible options with prefix GLFW_. The second argument is an int
     * that sets the value of the option
     */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float vertices[] = {
        // positions                      // colors
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    // top
    };

    unsigned int indices[] = {
        // 0, 1, 3, // first triangle
        // 1, 2, 3  // second triangle
        0, 1, 2};

    /* Create a buffer to pass data from the CPU to the GPU
     * Passing data between them is slow and costly so:
     *    1. We avoid it as much as possible
     *    2. When needed, we send as much data as possible at once.
     * The code below creates a buffer,
     */

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    // glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Now we need to instruct the vertex shader on how to read our vertex data.
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader customShader("shaders/vertex_shader.vert", "shaders/fragment_shader.frag");
    while (!glfwWindowShouldClose(window)) {
        // clear frame
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f); // A nice dark green
        glClear(GL_COLOR_BUFFER_BIT);

        // input
        process_input(window);

        customShader.use();

        // offset x by a time function
        float time_value = glfwGetTime();
        float h_offset = sin(time_value) / 2.0f;

        customShader.setFloat("h_offset", h_offset);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // check and call events, and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // As soon as the render loop is finished, properly delete all GLFW's resources and return.
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    /* Simple function to close the window if the escape key is pressed.
     * Uses glfwGetKey to record keyboard input, and sets glfw' "ShouldClose" to true.
     * GLFW_PRESSED is set when the key is pressed, otherwise it returns GLFW_RELEASE
     */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}