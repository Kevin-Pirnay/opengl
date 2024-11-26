#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>  // On macOS, OpenGL 3.2+ is supported directly without loaders like GLAD.


void create_and_push_schader(const char *const vertexShaderSource, const char *const fragmentShaderSource)
{
     unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link the shaders into a shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete the shaders as they're now linked into our program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}