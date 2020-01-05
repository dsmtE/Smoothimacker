#version 440 core

#define MAX_LIGHTS 128

uniform vec3 pointsLightsAmbiantColor[MAX_LIGHTS];

out vec4 FragColor;

flat in int InstanceID;

float max3 (vec3 v) {
  return max (max (v.x, v.y), v.z);
}

void main() {
    
    FragColor = vec4(pointsLightsAmbiantColor[InstanceID] / max3(pointsLightsAmbiantColor[InstanceID]), 1.0); 
}
