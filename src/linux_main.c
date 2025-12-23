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

/*void build_button(Arena* vertex_arena, f32 x_position, f32 y_position, f32 width, f32 height, f32 margin) {
    f32 inner_w = width - 2.0f * margin;
    f32 inner_h = height - 2.0f * margin;

    // Bottom row
    ST_Rectangle* r = arena_push(vertex_arena, sizeof(ST_Rectangle));
    *r = build_rect(x_position, y_position, margin, margin);        // Bottom-left
    r = arena_push(vertex_arena, sizeof(ST_Rectangle));
    *r = build_rect(x_position + margin, y_position, inner_w, margin);        // Bottom
    r = arena_push(vertex_arena, sizeof(ST_Rectangle));
    *r = build_rect(x_position + margin + inner_w, y_position, margin, margin);        // Bottom-right

    // Middle row
    r = arena_push(vertex_arena, sizeof(ST_Rectangle));
    *r = build_rect(x_position, y_position + margin, margin, inner_h);  // Left
    r = arena_push(vertex_arena, sizeof(ST_Rectangle));
    *r = build_rect(x_position + margin, y_position + margin, inner_w, inner_h);  // Center
    r = arena_push(vertex_arena, sizeof(ST_Rectangle));
    *r = build_rect(x_position + margin + inner_w, y_position + margin, margin,      inner_h);  // Right

    // Top row
    r = arena_push(vertex_arena, sizeof(ST_Rectangle));
    *r = build_rect(x_position, y_position + margin + inner_h, margin, margin);       // Top-left
    r = arena_push(vertex_arena, sizeof(ST_Rectangle));
    *r = build_rect(x_position + margin, y_position + margin + inner_h, inner_w, margin);   // Top
    r = arena_push(vertex_arena, sizeof(ST_Rectangle));
    *r = build_rect(x_position + margin + inner_w, y_position + margin + inner_h, margin, margin); // Top-right
}*/

int main() {
    GLFWwindow* window = opengl_initialize();
    
    GLuint button_shader = compile_button_shaders();

    Arena vertex_arena = {0};

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect_ratio = (f32)width / (f32)height;
    
    glOrtho(-aspect_ratio, aspect_ratio, -1.0, 1.0, -1.0, 1.0);

    ST_Rectangle* rect1 = arena_push(&vertex_arena, sizeof(ST_Rectangle));

    *rect1 = build_rect(0.0f, 0.0f, 1.0f, 0.5f);
    
    u32 buffer;
    glGenBuffers(1, &buffer);
    draw_vertex_arena_to_buffer(vertex_arena, buffer);

    f32 default_grey = 0.35f;
    
    // TODO: Make this update properly
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