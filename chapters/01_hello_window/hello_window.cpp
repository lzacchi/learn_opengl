#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

int main() {
    /* In the main function, we inigialyze GLFW with glfwInit, and after that we configure it
     * using glfwWindowHint. Its first argument indicates what option we want so select, based
     * on a large enum of possible options with prefix GLFW_. The second argument is an int
     * that sets the value of the option.
     */

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  for Mac OS X

    /* Next we are required to create a window object, which holds all the windowing data and
     * is required by most of GLFW's other functions. The glfwCreateWindow function requires
     * window width and height, as well as a title. This function returns a GLFWindo object.
     */
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // After that, we tell GLFW to make the context of this window the main context on the current thread.
    glfwMakeContextCurrent(window);

    /* After setting the context, initialize GLAD. This needs to be done before calling any OpenGL function, as
     * GLAD needs to provide the translation layer.
     */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /* Before rendering anything, OpenGL needs to know the size of the rendering window, so it knows how to display
     * data and more specifically the coordinates with respect to the window.
     */
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // adjust the viewport size whenever the window is resized.

    /* Create the "render loop", that keeps running until the window is closed.
     * This loop checks at the start of every iteration (frame) if the window should close,
     * the glfwPollEvents checks if any events are triggered (e.g.: keyboard inputs or mouse movements)
     * updates the window state and calls the corresponding functions (that can be registered via callbacks)
     * The glfwSwapBuffers will swap the colour buffer (a large 2D buffer containing colors for each pixel)
     * that was used to render this iteration and show it as output to the screen.
     */
    while (!glfwWindowShouldClose(window)) {
        /* Clear the screen at the start of every frame.
         * whenever glClear is called with GL_COLOR_BUFFER_BIT, the color is
         * set to the value configured in glClearColor.
         * glClearColor is a "state-setting" function, and glClear is a "state-using" function,
         * that uses the current state to retrieve the clearing color.
         */
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f); // A nice dark green.
        glClear(GL_COLOR_BUFFER_BIT);

        // input
        process_input(window);

        // rendering commands here

        // check and call events, and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // As soon as the render loop is finished, properly delete all GLFW's resources and return.
    glfwTerminate();
    return 0;
}
