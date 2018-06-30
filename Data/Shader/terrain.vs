#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 normal;
out vec3 toLight;
out vec3 toCamera;
out float fog_value;

// Uniforms
uniform vec3 lightPosition;
uniform mat4 MVP;
uniform mat4 MV;

uniform float fog_density;
uniform float fog_gradient;

void main(){
    
    // Output position of the vertex, in clip space : MVP * position
    gl_Position = MVP * vec4(vertexPosition_modelspace,1);
    
    UV = vertexUV * 128;
    normal = normalize((MV * vec4(vertexNormal, 0)).xyz);
    toLight = normalize(lightPosition - (MV * vec4(vertexPosition_modelspace, 1)).xyz);
    toCamera = -(MV * vec4(vertexPosition_modelspace, 1)).xyz;
    float distance = length(toCamera);
    toCamera /= distance;
    
    fog_value = clamp(exp(-pow(fog_density*distance, fog_gradient)), 0, 1);
}
