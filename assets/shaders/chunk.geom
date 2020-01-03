#version 440 core

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

// in
in VS_OUT {
    vec3 color;
    flat uint faceMask;
} gs_in[];

//out
out GS_OUT {
    vec3 color;
    flat uint faceMask;
    vec2 textCoord;
} gs_out;
 
// uniform  
// uniform mat3 NormalMatrix;
uniform mat4 MVPMatrix;

//    v5----- v4
//   /|      /|
//  v0------v1|
//  | |     | |
//  | |v6---|-|v7
//  |/      |/
//  v3------v2

const vec4 v0 = vec4(-1, 1, 1, 0) * 0.5;
const vec4 v1 = vec4(1, 1, 1, 0) * 0.5;
const vec4 v2 = vec4(1, -1, 1, 0) * 0.5;
const vec4 v3 = vec4(-1, -1, 1, 0) * 0.5;
const vec4 v4 = vec4(1, 1, -1, 0) * 0.5;
const vec4 v5 = vec4(-1, 1, -1, 0) * 0.5;
const vec4 v6 = vec4(-1, -1, -1, 0) * 0.5;
const vec4 v7 = vec4(1, -1, -1, 0) * 0.5;

void emitBox(vec4 position) {

    // up
    if( (gs_in[0].faceMask & 0x01) != 0) {
        gl_Position = MVPMatrix * (position + v5);
        gs_out.textCoord = vec2(0, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v4);
        gs_out.textCoord = vec2(1, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v0);
        gs_out.textCoord = vec2(0, 1);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v1);
        gs_out.textCoord = vec2(1, 1);
        EmitVertex();
        EndPrimitive();
    }

    // down
    if( (gs_in[0].faceMask & 0x02) != 0) {
        gl_Position = MVPMatrix * (position + v3);
        gs_out.textCoord = vec2(0, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v2);
        gs_out.textCoord = vec2(1, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v6);
        gs_out.textCoord = vec2(0, 1);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v7);
        gs_out.textCoord = vec2(1, 1);
        EmitVertex();
        EndPrimitive();
    }

    // left
    if( (gs_in[0].faceMask & 0x04) != 0) {
        gl_Position = MVPMatrix * (position + v5);
        gs_out.textCoord = vec2(0, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v0);
        gs_out.textCoord = vec2(1, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v6);
        gs_out.textCoord = vec2(0, 1);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v3);
        gs_out.textCoord = vec2(1, 1);
        EmitVertex();
        EndPrimitive();
    }

    // right
    if( (gs_in[0].faceMask & 0x08) != 0) {
        gl_Position = MVPMatrix * (position + v1);
        gs_out.textCoord = vec2(0, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v4);
        gs_out.textCoord = vec2(1, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v2);
        gs_out.textCoord = vec2(0, 1);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v7);
        gs_out.textCoord = vec2(1, 1);
        EmitVertex();
        EndPrimitive();
    }

    // front
    if( (gs_in[0].faceMask & 0x10) != 0) {
        gl_Position = MVPMatrix * (position + v0);
        gs_out.textCoord = vec2(0, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v1);
        gs_out.textCoord = vec2(1, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v3);
        gs_out.textCoord = vec2(0, 1);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v2);
        gs_out.textCoord = vec2(1, 1);
        EmitVertex();
        EndPrimitive();
    }
    // back
    if( (gs_in[0].faceMask & 0x20) != 0) {
        gl_Position = MVPMatrix * (position + v4);
        gs_out.textCoord = vec2(0, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v5);
        gs_out.textCoord = vec2(1, 0);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v7);
        gs_out.textCoord = vec2(0, 1);
        EmitVertex();
        gl_Position = MVPMatrix * (position + v6);
        gs_out.textCoord = vec2(1, 1);
        EmitVertex();
        EndPrimitive();
    }

}

void main() {
    gs_out.color = gs_in[0].color;
    gs_out.faceMask = gs_in[0].faceMask;
    emitBox(gl_in[0].gl_Position);
}