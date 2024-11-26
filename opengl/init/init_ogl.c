#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>  // On macOS, OpenGL 3.2+ is supported directly without loaders like GLAD.
#include <stdio.h>

#define WIDTH 800
#define Height 600

// Function to handle GLFW errors
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void init()
{
    // Initialize GLFW
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    // Set GLFW OpenGL version to 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, Height, "OpenGL Square", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // Set viewport size and register resize callback
    glViewport(0, 0, WIDTH, Height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}