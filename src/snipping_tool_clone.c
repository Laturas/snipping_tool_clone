#include "types.c"

const char* button_fragment_shader_source = 
    #include "shaders/button_frag_shader.glsl"
;
const char* button_vertex_shader_source =
    #include "shaders/button_vert_shader.glsl"
;

typedef struct Point {
    f32 x; f32 y; f32 z;
    f32 u; f32 v;
} Point, Vertex;

typedef struct Triangle {
    Point point_1;
    Point point_2;
    Point point_3;
} Triangle;

typedef struct Rectangle {
    Triangle triangles[2];
} Rectangle;

/**
* Position coordinate defined as the top left position
*/
Rectangle build_rect(f32 x_position, f32 y_position, f32 width, f32 height) {
    return (Rectangle){
        (Triangle){
            .point_1 = (Vertex){.x = x_position + 0.0f, .y = y_position + 0.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f}, // bottom left (?)
            .point_2 = (Vertex){.x = x_position + width, .y = y_position + height, .z = 0.0f, .u = 1.0f, .v = 1.0f}, // top right
            .point_3 = (Vertex){.x = x_position + 0.0f, .y = y_position + height, .z = 0.0f, .u = 0.0f, .v = 1.0f}, // top left
        },
        (Triangle){
            .point_1 = (Vertex){.x = x_position + 0.0f, .y = y_position + 0.0f, .z = 0.0f, .u = 0.0f, .v = 0.0f}, // bottom left
            .point_2 = (Vertex){.x = x_position + width, .y = y_position + height, .z = 0.0f, .u = 1.0f, .v = 1.0f}, // top right
            .point_3 = (Vertex){.x = x_position + width, .y = y_position + 0.0f, .z = 0.0f, .u = 1.0f, .v = 0.0f}, // bottom right
        },
    };
}

#define RECTANGLE_AREA(rect) (rect.width * rect.height)


// static_assert(sizeof(Triangle) == 36);