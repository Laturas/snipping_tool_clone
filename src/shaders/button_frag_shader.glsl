#version 410 core\n
out vec4 frag_colour;
uniform vec4 in_color;
uniform float border_width;
uniform float aspect;
in vec2 texCoord;
void main() {
   vec2 duv_dx = dFdx(vUV);
   vec2 duv_dy = dFdy(vUV);
   vec2 stretch;
   stretch.x = length(vec2(duv_dx.x, duv_dy.x));
   stretch.y = length(vec2(duv_dx.y, duv_dy.y));
   vec2 coords = texCoord;
   coords.x *= 2;
   frag_colour = vec4(fract(coords.x),texCoord.y,0.0f,1.0f);
}