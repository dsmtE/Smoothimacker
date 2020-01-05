#version 440 core

#define MAX_LIGHTS 128

out vec4 FragColor;

//uniform
uniform vec3 sunDir;
uniform vec3 sunColor;


uniform int nbOfPointsLight;
struct PointLight {
    vec3 pos;
    float constant;
    float linear;
    float quadratic;
    vec3 ambientColor;
    vec3 diffuseColor;
};

uniform PointLight pointsLights[MAX_LIGHTS];

in GS_OUT {
    vec3 color;
    vec2 textCoord;
    vec3 fragPos;
    vec3 normal;
} fs_in;

// calculates the color when using a point light.
vec3 CalcPointLight(vec3 fragPos, vec3 normal, PointLight light, float ambientStrength) {
    vec3 lightDir = normalize(light.pos - fragPos);
    // diffuse
    const int toonShadingLvl = 10;
    float diff = max(dot(normal, lightDir), 0.0);
    diff = floor(diff * toonShadingLvl) / float(toonShadingLvl);
    // attenuation
    float dist = length(light.pos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));    

    return attenuation * (light.ambientColor * ambientStrength + light.diffuseColor * diff);
}

void main() {

    // ambient
    float ambientStrength = 0.01;
    vec3 ambient = ambientStrength * sunColor;

    // combined textcordoColor
    vec3 colorFromTextCoord = vec3(fs_in.textCoord.x, fs_in.textCoord.y, 1.0);
    vec3 combinedColor = fs_in.color * 0.95 + colorFromTextCoord * 0.05;

    // diffuse 
    float diff = max(dot(fs_in.normal, normalize(-sunDir)), 0.0); // use a negative sign in front of sunDir because i defined sunDir as the vector coming from the sun
    const int toonShadingLvl = 10;
    diff = floor(diff * toonShadingLvl) / float(toonShadingLvl);
    vec3 diffuse = diff * sunColor;

    vec3 ptLights = vec3(0.0);
    for(int i = 0; i < nbOfPointsLight; i++) {
        ptLights += CalcPointLight(fs_in.fragPos, fs_in.normal, pointsLights[i], ambientStrength);    
    }

    FragColor = vec4(( ambient + diffuse + ptLights) * combinedColor , 1.0); 
}