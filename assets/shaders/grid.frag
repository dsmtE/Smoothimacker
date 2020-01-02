#version 440 core

uniform vec3 gridColor;
uniform int size;

in vec2 textCoord;
out vec4 FragColor;

float grid(vec2 uv, int size, float borderRatio) {
  vec2 grid = fract(uv*size);
  return (step(borderRatio,grid.x) * step(borderRatio,grid.y));
}

void main() {
    FragColor =  vec4( gridColor, 1 - grid(textCoord, size+1, 0.05f));
}

