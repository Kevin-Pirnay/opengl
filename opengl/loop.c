#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>  // On macOS, OpenGL 3.2+ is supported directly without loaders like GLAD.

typedef unsigned int uint;

void loop(const GLFWwindow *const window, const uint shaderProgram, const uint VAO)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        // Clear the screen with a color (background)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program and bind the VAO
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // Draw the square (two triangles)
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
}