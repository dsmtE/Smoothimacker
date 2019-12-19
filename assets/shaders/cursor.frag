#version 440 core

uniform vec3 cursorColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(cursorColor, 1.0f);
}