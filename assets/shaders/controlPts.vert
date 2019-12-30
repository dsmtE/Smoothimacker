#version 440 core

// in  
layout(location = 0) in vec3 cubeVertexRelativePos;
layout(location = 1) in vec3 cubePosition;

uniform mat4 MVPMatrix;

void main() {  
    gl_Position = MVPMatrix * vec4(cubePosition + cubeVertexRelativePos, 1.0);
}