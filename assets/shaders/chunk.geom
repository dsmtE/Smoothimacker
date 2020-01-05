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
    vec2 textCoord;
    vec3 fragPos;
    vec3 normal;
} gs_out;
 
// uniform
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelMatrix;

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

const vec3 nTop = vec3(0, 1, 0);
const vec3 nDown = vec3(0, -1, 0);
const vec3 nFront = vec3(0, 0, 1);
const vec3 nBack = vec3(0, 0, -1);
const vec3 nLeft = vec3(-1, 0, 0);
const vec3 nRight = vec3(1, 0, 0);

void emitBox(vec4 position) {

    // top
    if( (gs_in[0].faceMask & 0x01) != 0) {
        vec4 vertexPos[4] = vec4[4](position + v5, position + v4, position + v0, position + v1);
        vec2 textCoord[4] = vec2[4](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));
        vec3 normNorm = normalize(NormalMatrix * nTop); // in world space (using ModelMatrix)
        for(int i=0;i<4;++i) {
            gl_Position = MVPMatrix * vertexPos[i]; // in projected space
            gs_out.fragPos = vec3(ModelMatrix * vertexPos[i]); // in world space
            gs_out.textCoord = textCoord[i];
            gs_out.normal = normNorm;
            EmitVertex();
        }
        EndPrimitive();
    }

    // down
    if( (gs_in[0].faceMask & 0x02) != 0) {

        vec4 vertexPos[4] = vec4[4](position + v3, position + v2, position + v6, position + v7);
        vec2 textCoord[4] = vec2[4](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));
        vec3 normNorm = normalize(NormalMatrix * nDown);
        for(int i=0;i<4;++i) {
            gl_Position = MVPMatrix * vertexPos[i];
            gs_out.fragPos = vec3(ModelMatrix * vertexPos[i]);
            gs_out.textCoord = textCoord[i];
            gs_out.normal = normNorm;
            EmitVertex();
        }
        EndPrimitive();
    }

    // left
    if( (gs_in[0].faceMask & 0x04) != 0) {

        vec4 vertexPos[4] = vec4[4](position + v5, position + v0, position + v6, position + v3);
        vec2 textCoord[4] = vec2[4](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));
        vec3 normNorm = normalize(NormalMatrix * nLeft);
        for(int i=0;i<4;++i) {
            gl_Position = MVPMatrix * vertexPos[i];
            gs_out.fragPos = vec3(ModelMatrix * vertexPos[i]);
            gs_out.textCoord = textCoord[i];
            gs_out.normal = normNorm;
            EmitVertex();
        }
        EndPrimitive();
    }

    // right
    if( (gs_in[0].faceMask & 0x08) != 0) {

        vec4 vertexPos[4] = vec4[4](position + v1, position + v4, position + v2, position + v7);
        vec2 textCoord[4] = vec2[4](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));
        vec3 normNorm = normalize(NormalMatrix * nRight);
        for(int i=0;i<4;++i) {
            gl_Position = MVPMatrix * vertexPos[i];
            gs_out.fragPos = vec3(ModelMatrix * vertexPos[i]);
            gs_out.textCoord = textCoord[i];
            gs_out.normal = normNorm;
            EmitVertex();
        }
        EndPrimitive();
    }

    // front
    if( (gs_in[0].faceMask & 0x10) != 0) {

        vec4 vertexPos[4] = vec4[4](position + v0, position + v1, position + v3, position + v2);
        vec2 textCoord[4] = vec2[4](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));
        vec3 normNorm = normalize(NormalMatrix * nFront);
        for(int i=0;i<4;++i) {
            gl_Position = MVPMatrix * vertexPos[i];
            gs_out.fragPos = vec3(ModelMatrix * vertexPos[i]);
            gs_out.textCoord = textCoord[i];
            gs_out.normal = normNorm;
            EmitVertex();
        }
        EndPrimitive();

    }
    // back
    if( (gs_in[0].faceMask & 0x20) != 0) {

        vec4 vertexPos[4] = vec4[4](position + v4, position + v5, position + v7, position + v6);
        vec2 textCoord[4] = vec2[4](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1));
        vec3 normNorm = normalize(NormalMatrix * nBack);
        for(int i=0;i<4;++i) {
            gl_Position = MVPMatrix * vertexPos[i];
            gs_out.fragPos = vec3(ModelMatrix * vertexPos[i]);
            gs_out.textCoord = textCoord[i];
            gs_out.normal = normNorm;
            EmitVertex();
        }
        EndPrimitive();
    }

}

void main() {
    gs_out.color = gs_in[0].color;
    emitBox(gl_in[0].gl_Position);
    vec3 fragPos;
}