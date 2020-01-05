#version 440 core

#define MAX_LIGHTS 128

// in  
layout(location = 0) in vec3 cubeVertexPos;
layout(location = 1) in vec3 lightPos;

// uniform
uniform mat4 MVPMatrix;

void main() {  
    gl_Position = MVPMatrix * vec4(lightPos + cubeVertexPos, 1.0);
}