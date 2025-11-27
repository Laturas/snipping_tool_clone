#include "../includes/glad.c"
#include "../includes/glfw3.h"
#include <GL/glext.h>
#include <assert.h>

#include "snipping_tool_clone.c"

int main() {
    int init_error_code = glfwInit();
    assert(init_error_code);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hell", NULL, NULL);
    assert(window);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    Triangle triangles[2] = {
        (Triangle){
            .point_1 = (Vertex){.x = 0.0f, .y = 0.0f, .z = 0.0f},
            .point_2 = (Vertex){.x = 0.5f, .y = 0.5f, .z = 0.0f},
            .point_3 = (Vertex){.x = 0.0f, .y = 0.5f, .z = 0.0f},
        },
        (Triangle){
            .point_1 = (Vertex){.x = 0.0f, .y = 0.0f, .z = 0.0f},
            .point_2 = (Vertex){.x = 0.5f, .y = 0.5f, .z = 0.0f},
            .point_3 = (Vertex){.x = 0.5f, .y = 0.0f, .z = 0.0f},
        },
    };

    GLuint vs = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vs, 1, &button_vertex_shader_source, NULL );
    glCompileShader( vs );
    GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fs, 1, &button_fragment_shader_source, NULL );
    glCompileShader( fs );



    GLuint shader_program = glCreateProgram();
    glAttachShader( shader_program, fs );
    glAttachShader( shader_program, vs );
    glLinkProgram( shader_program );
    

    u32 buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glEnableVertexAttribArray(0);

    f32 default_grey = 0.35f;
    // Note: Move into loop on rerendering
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (float)width / (float)height;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUniform1f(glGetUniformLocation(shader_program, "aspect"), aspect);
        // glUniform1f(glGetUniformLocation(shader_program, "border_width"), 0.1f);

        i32 vertexColorLocation = glGetUniformLocation(shader_program, "in_color");
        // assert(vertexColorLocation != -1);
        glUseProgram( shader_program );
        glUniform4f(vertexColorLocation, default_grey, default_grey, default_grey, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}