#version 440 core

out vec4 FragColor;

//uniform
uniform vec3 sunDir;
uniform vec3 sunColor;


in GS_OUT {
    vec3 color;
    vec2 textCoord;
    vec3 fragPos;
    vec3 normal;
} fs_in;

void main() {

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * sunColor;

    // combined textcordoColor
    vec3 colorFromTextCoord = vec3(fs_in.textCoord.x, fs_in.textCoord.y, 1.0);
    vec3 combinedColor = fs_in.color * 0.95 + colorFromTextCoord * 0.05;

    // diffuse 
    float diff = max(dot(fs_in.normal, normalize(-sunDir)), 0.0); // use a negative sign in front of sunDir because i defined sunDir as the vector coming from the sun
    vec3 diffuse = diff * sunColor;

    FragColor = vec4(( ambient + diffuse) * combinedColor , 1.0); 
}