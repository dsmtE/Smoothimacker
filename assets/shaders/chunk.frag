#version 440 core

out vec4 FragColor;


in GS_OUT {
    flat uint type;
    flat uint faceMask;
    vec2 textCoord;
} fs_in;

// All components are in the range [0…1], including hue.
vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    vec3 typeColor = hsv2rgb(vec3(float(fs_in.type)/(2.0*16.0), 0.8, 0.8));
    vec3 colorFromTextCoord = vec3(fs_in.textCoord.x, fs_in.textCoord.y, 1.0f);
    FragColor = vec4(typeColor * 0.9 + colorFromTextCoord * 0.1, 1.0f);
}