#include "types.c"

const char* button_fragment_shader_source = 
    #include "shaders/button_frag_shader.glsl"
;
const char* button_vertex_shader_source =
    #include "shaders/button_vert_shader.glsl"
;


typedef struct Rectangle {
    f32 position_x;
    f32 position_y;
    f32 width;
    f32 height;
} Rectangle;

#define RECTANGLE_AREA(rect) (rect.width * rect.height)

typedef struct Point {
    f32 x;
    f32 y;
    f32 z;
} Point, Vertex;

typedef struct Triangle {
    Point point_1;
    Point point_2;
    Point point_3;
} Triangle;

// static_assert(sizeof(Triangle) == 36);