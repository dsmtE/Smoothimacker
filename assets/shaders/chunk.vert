#version 440 core

// in  
layout(location = 0) in uint index;  
layout(location = 1) in uint type; 
layout(location = 2) in uint faceMask;

// out
out VS_OUT { 
    flat uint type;
    flat uint faceMask;
} vs_out;

// uniform  
uniform int chunkSize;

vec3 indexToCoord(uint index, uint size) {
    float id = float(index);
    return vec3(mod(id, size), floor(id / (size * size)), mod(floor(id / size), size)); 
}

void main() {
    gl_Position = vec4(indexToCoord(index, chunkSize), 1);
    vs_out.faceMask = faceMask;
    vs_out.type = type;
}

