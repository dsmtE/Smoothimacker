#version 440 core

uniform vec3 controlPtsColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(controlPtsColor, 1.0f);
}