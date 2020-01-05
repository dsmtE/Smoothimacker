#version 440 core


// in  
layout(location = 0) in vec3 cubeVertexPos;
layout(location = 1) in vec3 lightPos;

flat out int InstanceID; 

// uniform
uniform mat4 MVPMatrix;

void main() {  
    gl_Position = MVPMatrix * vec4(lightPos + cubeVertexPos, 1.0);
    InstanceID = gl_InstanceID;
}