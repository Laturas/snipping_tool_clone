#include <assert.h>

#include "snipping_tool_clone.c"

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