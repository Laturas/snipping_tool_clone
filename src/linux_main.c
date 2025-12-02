#include "../includes/glad.c"
#include "../includes/glfw3.h"
#include <assert.h>

#include "snipping_tool_clone.c"
#include "arena.c"

void opengl_compilation_check(GLuint shader, u32 shader_type, i32 line) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        fprintf(stderr, "ERROR: %s shader compilation failed (check at line %d):\n%s\n", 
            (shader_type == GL_VERTEX_SHADER) ? "Vertex"
            : (shader_type == GL_FRAGMENT_SHADER) ? "Fragment"
            : "Unknown",
        line, infoLog);
    }
}
void opengl_linker_check(GLuint program, i32 line) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        fprintf(stderr, "ERROR: Linking step failed (check at line %d):\n%s\n", line, infoLog);
    }
}

GLuint compile_button_shaders() {
    GLuint vs = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vs, 1, &button_vertex_shader_source, NULL );
    glCompileShader( vs );
    GLuint fs = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fs, 1, &button_fragment_shader_source, NULL );
    glCompileShader( fs );
    
    opengl_compilation_check(vs, GL_VERTEX_SHADER, __LINE__);
    opengl_compilation_check(fs, GL_FRAGMENT_SHADER, __LINE__);
    
    GLuint shader_program = glCreateProgram();
    glAttachShader( shader_program, fs );
    glAttachShader( shader_program, vs );
    glLinkProgram( shader_program );
    opengl_linker_check(shader_program, __LINE__);
    return shader_program;
}

void draw_vertex_arena_to_buffer(Arena vertex_arena, u32 buffer) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    // fprintf(stderr, "Allocated: %lld\n", vertex_arena.first_unallocated_byte);
    glBufferData(GL_ARRAY_BUFFER, vertex_arena.first_unallocated_byte, vertex_arena.bytes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(f32) * 3));
    glEnableVertexAttribArray(1);
}

GLFWwindow* opengl_initialize() {
    int init_error_code = glfwInit();
    assert(init_error_code);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hell", NULL, NULL);
    assert(window);

    // Make the window's context current
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    return window;
}

int main() {
    GLFWwindow* window = opengl_initialize();
    
    GLuint button_shader = compile_button_shaders();

    Arena vertex_arena = {0};
    ST_Rectangle* rect1 = arena_push(&vertex_arena, sizeof(ST_Rectangle));
    ST_Rectangle* rect2 = arena_push(&vertex_arena, sizeof(ST_Rectangle));

    *rect1 = build_rect(0.0f, 0.0f, 0.5f, 0.5f);
    *rect2 = build_rect(-1.0f, -0.75f, 1.0f, 0.5f);
    
    u32 buffer;
    glGenBuffers(1, &buffer);
    draw_vertex_arena_to_buffer(vertex_arena, buffer);

    f32 default_grey = 0.35f;
    
    // TODO: Make this update properly
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect_ratio = (f32)width / (f32)height;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(button_shader);
        glUniform1f(glGetUniformLocation(button_shader, "border_width"), 0.02f);
        glUniform1f(glGetUniformLocation(button_shader, "aspect"), aspect_ratio);
        i32 vertexColorLocation = glGetUniformLocation(button_shader, "in_color");
        glUniform4f(vertexColorLocation, default_grey, default_grey, default_grey, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, vertex_arena.first_unallocated_byte / sizeof(Vertex));

        // Swap front and back buffers (we don't have a back buffer)
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Optional in this case
    arena_destroy(&vertex_arena);
    glfwTerminate();
    return 0;
}