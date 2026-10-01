#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader/shader.h>
#include <stb_image/stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

const int WINDOW_WIDTH  = 1024;
const int WINDOW_HEIGHT = 576;

float mix_attenuation = 0.0f;

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

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    // clang-format off
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    /* In order to map a texture to the triangle, each of its vertices
    * need to be mapped to a part of the texture it corresponds to.
    * As such, each vertex needs a texture coordinate associated with it,
    * specifying what part of the texture to sample from. The fragment shader
    * then uses this coordinates to interpolate the texture.
    *
    * Texture coordinates range from (0,0) on the lower-left corner
    * to (1,1) on the top-right corner. 
    */

    // clang-format on

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

    unsigned int main_texture, second_texture;
    glGenTextures(1, &main_texture);
    glBindTexture(GL_TEXTURE_2D, main_texture);

    /* Texture wrapping can defined per axis (s, t, and r in case of 3D textures)
     * Available options are:
     *  GL_REPEAT: Default behaviour: repeats textures
     *  GL_MIRRORED_REPEAT: Same as GL_REPEEAT but mirrors the image with each repeat
     *  GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1. Higher coordinates become stretched
     *  GL_CLAMP_TO_BORDER: Coordinates outside the range are given a user-specified border color
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // If GL_CLAMP_TO_BORDER is used, a border colour needs to be defined:
    // float border_colour[] = {1.0f, 1.0f, 0.0f, 1.0f};
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_colour);

    /* Texture filtering can also be defined and these are the most relevant options:
     *  GL_NEAREST: aka nearest neighbour or point filtering. The default method, OpenGL selected the texel
     *              that center is closest to the texture coordinate.
     *  GL_LINEAR:  aka (bi)linear filtering. Takes an interpolated value from the texture coordinate's neighouring
     *              texels, approximating a colour between them.
     * Texture filtering can have different values when scaling upwards or downwards.
     *
     * The same options can be set to interpolate between two mipmaps:
     * There is no need to set up a Mipmap option on the MAG_Filter, since mipmaps are not applied
     * when upscaling a texture. Doing so will generate an invalid enum error.
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // loading the texture using the stb_image library
    // OpenGL expects the y0.0 to be on the bottom left of the image.
    // Images, however, decided that they wanted y0.0 to be on top left.
    // They can be gently reminded of their bad decision with the following statement:
    stbi_set_flip_vertically_on_load(true);
    int            tex_width, tex_height, nrChannels;
    unsigned char* main_tex = stbi_load("textures/container.jpg", &tex_width, &tex_height, &nrChannels, 0);
    if (main_tex) {

        glTexImage2D(
            GL_TEXTURE_2D, 0,
            GL_RGB,
            tex_width, tex_height,
            0, GL_RGB,
            GL_UNSIGNED_BYTE, main_tex);
        glGenerateMipmap(GL_TEXTURE_2D);

    } else {
        std::cout << "Failed to load main texture" << std::endl;
    }
    stbi_image_free(main_tex);

    glGenTextures(1, &second_texture);
    glBindTexture(GL_TEXTURE_2D, second_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char* second_tex = stbi_load("textures/awesomeface.png", &tex_width, &tex_height, &nrChannels, 0);
    if (second_tex) {
        glTexImage2D(
            GL_TEXTURE_2D, 0,
            GL_RGB,
            tex_width, tex_height,
            0, GL_RGBA,
            GL_UNSIGNED_BYTE, second_tex);
        glGenerateMipmap(GL_TEXTURE_2D);

    } else {
        std::cout << "Failed to load second texture" << std::endl;
    }
    stbi_image_free(second_tex);

    // glBufferData is a function specifically targeted to copy user-defined data into the currently
    // bound buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Now we need to instruct the vertex shader on how to read our vertex data.
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    Shader customShader("shaders/vertex_shader.vert", "shaders/fragment_shader.frag");
    while (!glfwWindowShouldClose(window)) {
        // clear frame
        glClearColor(0.2f, 0.3f, 0.3f, 0.1f); // A nice dark green
        glClear(GL_COLOR_BUFFER_BIT);

        // input
        process_input(window);

        customShader.use();
        glUniform1i(glGetUniformLocation(customShader.ID, "texture1"), 0);
        customShader.setInt("texture2", 1);
        customShader.setFloat("mix_attenuation", mix_attenuation);

        // offset x by a time function
        // float time_value = glfwGetTime();
        // float h_offset   = sin(time_value) / 2.0f;

        // customShader.setFloat("h_offset", h_offset);

        // Apply transformation matrix:
        // Orders really matter here: Scale first, then rotate, then translate
        glm::mat4 transform = glm::mat4(1.0f);
        transform           = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        transform           = glm::rotate(transform, float(glfwGetTime()), glm::vec3(0.0, 0.0, 1.0));
        transform           = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));

        unsigned int transform_location = glGetUniformLocation(customShader.ID, "transform");
        glUniformMatrix4fv(transform_location, 1, GL_FALSE, glm::value_ptr(transform));

        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding it
        glBindTexture(GL_TEXTURE_2D, main_texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, second_texture);
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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
    /* Simple function to close the window if the escape key is pressed.
     * Uses glfwGetKey to record keyboard input, and sets glfw' "ShouldClose" to true.
     * GLFW_PRESSED is set when the key is pressed, otherwise it returns GLFW_RELEASE
     */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        mix_attenuation += 0.01f;
        if (mix_attenuation >= 1.0f) {
            mix_attenuation = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mix_attenuation -= 0.01f;
        if (mix_attenuation <= 0.0f) {
            mix_attenuation = 0.0f;
        }
    }
}
