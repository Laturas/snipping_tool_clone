#include "types.c"
#include "../includes/glad.c"
#include "../includes/glfw3.h"

#include "arena.c"

// const char* button_fragment_shader_source = 
//     #include "shaders/button_frag_shader.glsl"
// ;

#define SHADER(x) "#version 410 core\n" #x

const char* button_fragment_shader_source = SHADER(
    out vec4 frag_colour;
    uniform vec4 in_color;
    uniform float border_width;
    in float stretch_x;
    in vec2 texCoord;
    void main() {
        vec2 duv_dx = dFdx(texCoord);
        vec2 duv_dy = dFdy(texCoord);
        vec2 stretch;
        stretch.x = length(vec2(duv_dx.x, duv_dy.x));
        stretch.y = length(vec2(duv_dx.y, duv_dy.y));
        vec2 coords = texCoord;
        coords.x *= stretch_x;
        frag_colour = vec4(fract(coords.x),fract(coords.y),0.0f,1.0f);
    }
);

const char* button_vertex_shader_source =
    #include "shaders/button_vert_shader.glsl"
;

typedef struct Point {
    f32 x; f32 y; f32 z;
    f32 u; f32 v; f32 stretch;
} Point, Vertex;

typedef struct Triangle {
    Point point_1;
    Point point_2;
    Point point_3;
} Triangle;

typedef struct ST_Rectangle {
    Triangle triangles[2];
} ST_Rectangle;

/**
* Position coordinate defined as the top left position
*/
ST_Rectangle build_rect(f32 x_position, f32 y_position, f32 width, f32 height) {
    return (ST_Rectangle){
        (Triangle){
            .point_1 = (Vertex){.x = x_position + 0.0f, .y = y_position + 0.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f, .stretch = width / height}, // bottom left (?)
            .point_2 = (Vertex){.x = x_position + width, .y = y_position + height, .z = 0.0f, .u = 1.0f, .v = 1.0f, .stretch = width / height}, // top right
            .point_3 = (Vertex){.x = x_position + 0.0f, .y = y_position + height, .z = 0.0f, .u = 0.0f, .v = 1.0f, .stretch = width / height}, // top left
        },
        (Triangle){
            .point_1 = (Vertex){.x = x_position + 0.0f, .y = y_position + 0.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f, .stretch = width / height}, // bottom left
            .point_2 = (Vertex){.x = x_position + width, .y = y_position + height, .z = 0.0f, .u = 1.0f, .v = 1.0f, .stretch = width / height}, // top right
            .point_3 = (Vertex){.x = x_position + width, .y = y_position + 0.0f, .z = 0.0f, .u = 1.0f, .v = 0.0f, .stretch = width / height}, // bottom right
        },
        // width = 2
        // height = 1
    };
}

#define RECTANGLE_AREA(rect) (rect.width * rect.height)

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL); /* Position */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(f32) * 3)); /* UV */
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(f32) * 5)); /* Stretch */
    glEnableVertexAttribArray(2);
}

// static_assert(sizeof(Triangle) == 36);