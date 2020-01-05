#version 440 core

#define MAX_LIGHTS 128

out vec4 FragColor;

//uniform
uniform vec3 camPos;
uniform vec3 sunDir;
uniform vec3 sunColor;
uniform float dayMode;

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

    //ambiant
    const vec3 ambiant = light.ambientColor * ambientStrength;

    // diffuse
    const int toonShadingLvl = 8;
    float diffuseIntensity = max(dot(normal, lightDir), 0.0);
    diffuseIntensity = floor(diffuseIntensity * toonShadingLvl) / float(toonShadingLvl);
    const vec3 diffuse = diffuseIntensity * light.diffuseColor;
    ;
    // specular
    float specularStrength = 0.3;
    vec3 viewDir = normalize(camPos - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, fs_in.normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
    vec3 specular = specularStrength * spec * light.ambientColor;  

    // attenuation
    float dist = length(light.pos - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));    

    return attenuation * ( ambiant + diffuse + specular);
}

void main() {

    // calcSun color
    vec3 globalLightColor = mix( vec3(0.03, 0.015, 0.20), sunColor, dayMode) * dot(normalize(vec3(0,1,0)), -sunDir) ;
    // ambient
    float ambientStrength = 0.01;
    vec3 ambient = ambientStrength *  globalLightColor;

    // combined textcordoColor
    vec3 colorFromTextCoord = vec3(fs_in.textCoord.x, fs_in.textCoord.y, 1.0);
    vec3 combinedColor = mix(fs_in.color, colorFromTextCoord , 0.01);

    // diffuse 
    float diffuseIntensity = clamp(dot(fs_in.normal, normalize(-sunDir)), 0.0, 1.0); // use a negative sign in front of sunDir because i defined sunDir as the vector coming from the sun
    vec3 diffuse = diffuseIntensity * globalLightColor;

    // specular
    float specularStrength = 0.3;
    vec3 viewDir = normalize(camPos - fs_in.fragPos);
    vec3 reflectDir = reflect(normalize(sunDir), fs_in.normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
    vec3 specular = specularStrength * spec * globalLightColor;  

    // points lights
    vec3 ptLights = vec3(0.0);
    for(int i = 0; i < nbOfPointsLight; i++) {
        ptLights += CalcPointLight(fs_in.fragPos, fs_in.normal, pointsLights[i], ambientStrength);    
    }

    FragColor = vec4(( ambient + diffuse + specular + ptLights) * combinedColor , 1.0); 
}