#version 440 core

// in  
layout(location = 0) in vec3 position;  
layout(location = 1) in vec3 color; 


// uniform  
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// out   
out vec3 vColor;  

void main() {  

    mat4 MVPMatrix = projection * view * model;

    gl_Position = MVPMatrix * vec4(position, 1.0);
    vColor = color;

}