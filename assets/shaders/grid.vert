#version 440 core
 
layout(location = 0) in vec2 pos;  

uniform mat4 MVPMatrix;
uniform int size;
out vec2 textCoord;

void main() {  
    gl_Position = MVPMatrix * vec4((size+1) * pos.x - 0.5f, - 0.52f, (size+1) * pos.y - 0.5f , 1.0);
    textCoord = pos;
}