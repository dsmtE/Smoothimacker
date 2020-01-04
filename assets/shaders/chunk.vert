#version 440 core

// in  
layout(location = 0) in uvec3 pos;  
layout(location = 1) in vec3 color; 
layout(location = 2) in uint faceMask;

// out
out VS_OUT { 
    vec3 color;
    flat uint faceMask;
} vs_out;

void main() {
    gl_Position = vec4(pos, 1);
    vs_out.faceMask = faceMask;
    vs_out.color = color;

}

